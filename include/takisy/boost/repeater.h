#ifndef repeater_h_20170509
#define repeater_h_20170509

#include <atomic>
#include <map>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <takisy/core/function_traits.h>
#include <takisy/core/handler.h>
#include <takisy/core/reflect.h>
#include <takisy/core/serialize.h>
#include <takisy/core/deserialize.h>

class repeater {
    struct request_handler {
        virtual ~request_handler(void) {}
        virtual std::string handle(void) {
            throw std::runtime_error("incomplate interface `handle().'");
        }
        virtual std::string handle(const std::string& buffer) {
            throw std::runtime_error("incomplate interface `handle(string).'");
        }
    };

    struct response_handler {
        virtual ~response_handler(void) {}
        virtual void handle(const std::string& buffer) = 0;
    };

    template <typename Lambda>
    struct lambda_request_handler_0 : public request_handler {
        Lambda lambda;
        lambda_request_handler_0(Lambda lambda) : lambda(lambda) {}
    public:
        std::string handle(void) override {
            return serialize::bin().serialize(lambda()).data();
        }
    };

    template <typename Lambda>
    struct lambda_request_handler_1 : public request_handler {
        Lambda lambda;
        lambda_request_handler_1(Lambda lambda) : lambda(lambda) {}
    public:
        std::string handle(const std::string& buffer) override {
            typedef function_traits<Lambda> traits;
            typedef typename traits::template pure_arg<0>::type body_type;
            body_type body = deserialize::bin(buffer).deserialize<body_type>();
            return serialize::bin().serialize(lambda(body)).data();
        }
    };

    template <typename Lambda>
    struct lambda_response_handler : public response_handler {
        Lambda lambda;
        lambda_response_handler(Lambda lambda) : lambda(lambda) {}
    public:
        void handle(const std::string& buffer) override {
            typedef function_traits<Lambda> traits;
            typedef typename traits::template pure_arg<0>::type body_type;
            lambda(deserialize::bin(buffer).deserialize<body_type>());
        }
    };

    template <int Arity>
    struct MakeRequestHandler {
        static_assert(Arity == 0 || Arity == 1, "invalid arity.");
    };

public:
    static constexpr const unsigned int kMagicNumber = 0x28a3b703;

    struct header {
        unsigned int magic_number, msgid, seqid, length;
    };

    DECLARE_HANDLER(onClose);

public:
    repeater(boost::asio::ip::tcp::socket& socket)
        : repeater(socket, 60)
    {}

    repeater(boost::asio::ip::tcp::socket& socket, unsigned int timeout)
        : timeout(timeout), socket(socket), katimer(socket.get_io_service())
    {}

   ~repeater(void) {}

private:
    repeater(const repeater&) = delete;
    repeater& operator=(const repeater&) = delete;

public:
    template <typename Lambda>
    void on_request(unsigned int msgid, Lambda lambda) {
        request_handlers[msgid].reset(make_request_handler(lambda));
    }

public:
    template <typename Lambda>
    void request(unsigned int msgid, Lambda lambda) {
        typedef lambda_response_handler<Lambda> response_handler;
        header h = { kMagicNumber, msgid, seqid(), 0 };
        send(serialize::bin().serialize(h).data());
        response_handlers[h.seqid].reset(new response_handler(lambda));
    }

    template <typename Request, typename Lambda>
    void request(unsigned int msgid, const Request& request, Lambda lambda) {
        typedef lambda_response_handler<Lambda> response_handler;
        std::string body = serialize::bin().serialize(request).data();
        header h = { kMagicNumber, msgid, seqid(), (unsigned int)body.size() };
        send(serialize::bin().serialize(h).data() + body);
        response_handlers[h.seqid].reset(new response_handler(lambda));
    }

public:
    void run(void) {
        using namespace boost::asio::error;
        std::shared_ptr<std::string> buffer(new std::string(sizeof(header), 0));
        keep_alive();
        boost::asio::async_read(socket, as_mutable_buffer(buffer),
        [this, buffer](const boost::system::error_code& ec,
                       unsigned int read_bytes) {
            if (ec) {
                if (ec.value() != operation_aborted)
                    close();
            } else if (read_bytes != buffer->size()) {
                close();
            } else {
                header h = deserialize::bin(*buffer).deserialize<header>();
                if (h.magic_number != kMagicNumber) {
                    close();
                } else if (h.length == 0) {
                    if (is_request_header(h)
                        && exists_handler(request_handlers, h.msgid)) {
                        std::string body = request_handlers[h.msgid]->handle();
                        h = as_response_header(h, body.size());
                        send(serialize::bin().serialize(h).data() + body);
                    }
                    run();
                } else {
                    buffer->resize(h.length);
                    boost::asio::async_read(socket, as_mutable_buffer(buffer),
                    [this, h, buffer](const boost::system::error_code& ec,
                                      unsigned int read_bytes) {
                        if (ec) {
                            if (ec.value() != operation_aborted)
                                close();
                        } else if (read_bytes != buffer->size()) {
                            close();
                        } else if (is_request_header(h)) {
                            if (exists_handler(request_handlers, h.msgid)) {
                                std::string body =
                                    request_handlers[h.msgid]->handle(*buffer);
                                header rh = as_response_header(h, body.size());
                                send(serialize::bin().serialize(rh).data() +
                                     body);
                            }
                        } else if (exists_handler(response_handlers, h.seqid)) {
                            response_handlers[h.seqid]->handle(*buffer);
                            response_handlers.erase(h.seqid);
                        }
                        run();
                    });
                }
            }
        });
    }

private:
    void keep_alive(void) {
        katimer.cancel();
        katimer.expires_from_now(boost::posix_time::seconds(timeout));
        katimer.async_wait(
            [this](const boost::system::error_code& ec) {
                if (!ec || ec.value() != boost::asio::error::operation_aborted)
                    close();
            });
    }

    void close(void) {
        if (socket.is_open()) {
            socket.cancel();
            socket.close();
            katimer.cancel();
            onCloseHandle();
        }
    }

    void send(std::string&& buffer) {
        std::shared_ptr<std::string> cpbuff(new std::string(std::move(buffer)));
        boost::asio::async_write(socket, as_mutable_buffer(cpbuff),
            [this, cpbuff](const boost::system::error_code& ec,
                           unsigned int read_bytes) {
                if (ec) {
                    if (ec.value() != boost::asio::error::operation_aborted)
                        close();
                } else if (read_bytes != cpbuff->size())
                    close();
            });
    }

private:
    static boost::asio::mutable_buffer
    as_mutable_buffer(const std::shared_ptr<std::string>& str) {
        return boost::asio::mutable_buffer((void*)str->data(), str->size());
    }

    template <typename Handlers>
    static bool exists_handler(const Handlers& handlers, unsigned int id) {
        return exists_handler(handlers, id);
    }

    static bool is_request_header(const header& h) {
        return (h.msgid & 0x80000000) == 0;
    }

    static header as_response_header(header h, unsigned int length) {
        h.msgid |= 0x80000000;
        h.length = length;
        return h;
    }

    static unsigned int seqid(void) {
        static std::atomic<unsigned int> seqid_generator(1);
        unsigned int seqid = seqid_generator.fetch_add(1);

        if (seqid > 0xffff0000)
            seqid_generator.store(1);

        return seqid;
    }

    template <typename Lambda>
    static request_handler* make_request_handler(Lambda lambda) {
        return MakeRequestHandler<function_traits<Lambda>::arity>::make(lambda);
    }

private:
    unsigned int timeout;
    boost::asio::ip::tcp::socket& socket;
    boost::asio::deadline_timer katimer;
    std::map<unsigned int, std::shared_ptr<request_handler>> request_handlers;
    std::map<unsigned int, std::shared_ptr<response_handler>> response_handlers;
};

REFLECT_ADAPT_STRUCT(repeater::header, magic_number, msgid, seqid, length);

template <>
struct repeater::MakeRequestHandler<0> {
    template <typename Lambda>
    static request_handler* make(Lambda lambda) {
        return new lambda_request_handler_0<Lambda>(lambda);
    }
};

template <>
struct repeater::MakeRequestHandler<1> {
    template <typename Lambda>
    static request_handler* make(Lambda lambda) {
        return new lambda_request_handler_1<Lambda>(lambda);
    }
};

#endif // repeater_h_20170509
