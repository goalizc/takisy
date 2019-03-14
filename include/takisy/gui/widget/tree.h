#ifndef tree_h_20170328
#define tree_h_20170328

class tree : public widget
{
    class implement;

public:
    tree(void);
   ~tree(void);

public:

private:
    class implement* impl_;
};

#endif // tree_h_20170328
