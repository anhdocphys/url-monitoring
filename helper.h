#ifndef HELPER_H
#define HELPER_H
#include <memory>
#include <set>

typedef std::string String;

struct Row {
    String url;
    String main_cat;
    String sub_cat_first;
    String sub_cat_second;
};

typedef std::shared_ptr<Row> RowPtr;

class CategoryNode {
public:
    CategoryNode(unsigned int i, const String& s, unsigned int j):
        id(i),
        title(s),
        parent_id(j),
        children()
    {
    }
    ~CategoryNode(){}
    inline String get_title() const {
        return title;
    }
    inline unsigned int get_id() const {
        return id;
    }
    inline unsigned int get_parent_id() const {
        return parent_id;
    }
    inline bool is_root() {
        return parent_id == 0;
    }
    inline void add_child(unsigned int child_id) {
        children.insert(child_id);
    }
    inline bood has_child(unsigned int child_id) {
        return children.count(child_id);
    }

private:
    unsigned int id;
    String title;
    unsigned int parent_id;
    std::set<unsigned int> children;
};

void normalize_text(String& txt, bool should_cap);



#endif // HELPER_H
