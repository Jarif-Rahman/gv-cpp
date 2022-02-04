#include <fstream>
#include <string>
#include <vector>
#include <map>

class graph{
    public:
        class node{
            friend class graph;
            private:
                int id;
            public:
                std::map<std::string, std::string> attr, attrq;
        };

        class edge{
            public:
                node *a, *b;
                std::map<std::string, std::string> attr, attrq;
        };
    private:
        std::vector<edge*> edges;
        std::vector<node*> nodes;

        template<typename type>
        std::string gen_attr(type* entity){
            std::string out = "[";
            for(auto [a, b]: entity->attr){
                out+=a+"="+b+",";
            }
            for(auto [a, b]: entity->attrq){
                out+=a+"=\""+b+"\",";
            }
            if(!(entity->attr.empty()) || !(entity->attrq.empty())) out.pop_back();
            out+="]";
            if(out.size() == 2) return "";
            return out;
        }

    public:
        std::map<std::string, std::string> attr, attrq;
        edge* edgeDefaults = new edge;
        node* nodeDefaults = new node;

        graph(){
        }
        graph(std::map<std::string, std::string> _attr, std::map<std::string, std::string> _attrq){
            attr = _attr;
            attrq = _attrq;
        }

        node* new_node(){
            node* nd = new node;
            nd->id = nodes.size();
            nodes.push_back(nd);
            return nd;
        }
        node* new_node(std::map<std::string, std::string> _attr, std::map<std::string, std::string> _attrq){
            node* nd = new node;
            nd->attr = _attr;
            nd->attrq = _attrq;
            nd->id = nodes.size();
            nodes.push_back(nd);
            return nd;
        }

        edge* new_edge(node *a, node *b){
            edge* e = new edge;
            e->a = a;
            e->b = b;
            edges.push_back(e);
            return e;
        }
        edge* new_edge(node *a, node *b, std::map<std::string, std::string> _attr, std::map<std::string, std::string> _attrq){
            edge* e = new edge;
            e->a = a;
            e->b = b;
            e->attr = _attr;
            e->attrq = _attrq;
            edges.push_back(e);
            return e;
        }

        std::string generate_graphviz(bool readable = false, std::string indent = "    "){
            std::string begin = "";
            std::string end = "";
            if(readable) begin = indent, end = "\n";

            std::string out = "digraph G{"+end;

            std::string attrs = gen_attr(this);
            if(!attrs.empty())out+=begin+"graph"+attrs+";"+end;

            attrs = gen_attr(nodeDefaults);
            if(!attr.empty()) out+=begin+"node"+attrs+";"+end;

            attrs = gen_attr(edgeDefaults);
            if(!attr.empty()) out+=begin+"edge"+attrs+";"+end;

            for(int i = 0; i < nodes.size(); i++){
                out+=begin+std::to_string(nodes[i]->id)+gen_attr(nodes[i])+";"+end;
            }
            for(int i = 0; i < edges.size(); i++){
                out+=begin+std::to_string(edges[i]->a->id)+"->"+std::to_string(edges[i]->b->id)+gen_attr(edges[i])+";"+end;
            }
            out+="}";
            return out;
        }

};

class Gif{
    private:
        std::vector<std::string> imgs;
    public:
        void add(std::string img){
            imgs.push_back(img);
        }
        void generate(std::string output, int delay=20, std::string format="jpg", std::string graphviz="dot"){
            if(imgs.empty()) return;
            std::string cmd = "convert -delay " + std::to_string(delay) + " -loop 0 ";
            for(int i = 0; i < imgs.size(); i++){
                std::ofstream F(".tmp.gv");
                F << imgs[i];
                F.close();
                system((graphviz + " -T" + format + " .tmp.gv -o .tmp-" + std::to_string(i) + "." + format).c_str());
                system("rm .tmp.gv");
                cmd+=" .tmp-" + std::to_string(i) + "." + format + " ";
            }
            cmd+=output;
            system(cmd.c_str());
            for(int i = 0; i < imgs.size(); i++){
                system(("rm .tmp-" + std::to_string(i) + "." + format).c_str());
            }
        }
};