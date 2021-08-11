// Author - Ritwik Sahani
// Roll No. - EE18BTECH11038

#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <bitset>


using namespace std;

bool compfn(pair<char, int> &a, pair<char, int> &b) { return a.second < b.second; }

class data
{
private:
    string content;

public:
    data() {}

    data(string str) { content = str; }

    string get_content() { return content; }

    void set_content(string s) { content = s; }

    int size() { return content.length(); }

    void read_from_file(const string file_name)
    {
        stringstream buffer;
        ifstream f(file_name);
        buffer << f.rdbuf();
        content = buffer.str();
    }

    void write_to_text_file(const string file_name)
    {
        stringstream buffer(content);
        ofstream outfile(file_name);
        outfile << buffer.rdbuf();
    }

    void write_to_binary_file(const string filename)
    {
        int len = content.length(), len_pad = (64- (len + 6) % 64);
        string padded = bitset<6>(len_pad).to_string() + content;
        for (int i = 0; i < len_pad; i++)
        {
            padded += '0';
        }

        std::ofstream f;
        f.open(filename, std::ios_base::out | std::ios_base::binary);

        for (int i = 0; i < (int)padded.length() / 64; i++)
        {
            bitset<64> b(string(padded.substr(i * 64, 64)));
            auto ul = b.to_ullong();
            f.write(reinterpret_cast<char *>(&ul), sizeof(ul));
        }
        f.close();
    }

    unordered_map<char, int> frequency()
    {
        unordered_map<char, int> freq;
        for (char c : content)
            freq[c]++;
        return freq;
    }
};

struct node
{
    char symbol = '\0';
    int frqncy;
    node *next = nullptr;
    node *left = nullptr;
    node *right = nullptr;

public:
    node(char c, int f = 0)
    {
        symbol = c;
        frqncy = f;
    }
    node() {}
    ~node()
    {
        delete left;
        delete right;
    }
};

class tree
{
public:
    node *root = nullptr;

    ~tree()
    {
        delete root;
    }

    void place(char c, int d, node *&start, bool &placed)
    {
        if (placed)
            return;
        if (start == nullptr)
            start = new node;
        if (start->symbol)
            return;

        if (d == 0)
        {
            start->symbol = c;
            placed = true;
            return;
        }
        if (!placed)
            place(c, d - 1, start->left, placed);
        if (!placed)
            place(c, d - 1, start->right, placed);
    }

    void place(char c, int d)
    {
        bool placed = false;
        place(c, d, root, placed);
    }

    void traversal(node *start, string prefix, unordered_map<char, string> &code_map)
    {
        if (!start)
            return;
        if (!(start->left || start->right))
        {
            code_map[start->symbol] = prefix;
            return;
        }

        prefix.push_back('0');
        traversal(start->left, prefix, code_map);
        prefix.pop_back();

        prefix.push_back('1');
        traversal(start->right, prefix, code_map);
        prefix.pop_back();
        return;
    }

    unordered_map<char, string> give_codes()
    {
        unordered_map<char, string> code_map;
        traversal(root, "", code_map);
        return code_map;
    }

    void decode(stringstream &comp, stringstream &decomp)
    {
        char c;
        if (comp >> c)
        {
            comp.unget();
            node *start = root;
            while (!(start->symbol))
            {
                comp >> c;
                start = (c == '0' ? start->left : start->right);
            }
            decomp << start->symbol;
            decode(comp, decomp);
        }
    }

    void place(stringstream &s, char symb, node *&start)
    {
        if (start == nullptr)
            start = new node;

        char c;
        if (s >> c)
        {
            if (c == '0')
                place(s, symb, start->left);
            else
                place(s, symb, start->right);
        }
        else
            start->symbol = symb;
    }

    void place(stringstream &s, char symb)
    {
        place(s, symb, root);
    }
};

class compressor
{
private:
    data *input_file;
    vector<pair<char, int> > freq_vec_stored;

public:
    compressor(data *&inp_file)
    {
        input_file = inp_file;
    }

    vector<pair<char, int> > get_ordered_freq(bool rev = false)
    {
        unordered_map<char, int> freq = input_file->frequency();
        vector<pair<char, int> > freq_vec(freq.begin(), freq.end());
        sort(freq_vec.begin(), freq_vec.end(), compfn);
        double entropy= 0, prob;
        int len = input_file->size();
        for(int i = 0; i<freq_vec.size(); i++){
            cout<< "Freq of "<<freq_vec[i].first << ": "<<freq_vec[i].second<<"\n";
            prob = (double)freq_vec[i].second/len;
            entropy -= prob * log2(prob);
        }
        cout<< "Entropy of given file = "<<entropy<<"\n\n";
        return freq_vec;
    }

    node *huffman_build(node *&head)
    {
        if (head->next == nullptr)
            return head;

        node *temp = new node;
        temp->left = head;
        temp->right = head->next;
        temp->frqncy = temp->left->frqncy + temp->right->frqncy;

        node *here = head;
        node *prev = nullptr;
        while (!(here == nullptr || here->frqncy > temp->frqncy))
        {
            prev = here;
            here = here->next;
        }
        prev->next = temp;
        temp->next = here;
        head = head->next->next;
        return huffman_build(head);
    }

    tree *compress(const string coding_type)
    {
        tree *tree_ptr = new tree;

        if (coding_type == "huffman")
        {
            if (freq_vec_stored.empty())
                freq_vec_stored = get_ordered_freq();

            char c;
            int f, alph = freq_vec_stored.size();
            node *head = nullptr, *cur, *prev = nullptr;

            for (int i = 0; i < alph; i++)
            {
                c = freq_vec_stored[i].first;
                f = freq_vec_stored[i].second;
                cur = new node(c, f);
                if (prev == nullptr)
                    head = cur;
                else
                    prev->next = cur;
                prev = cur;
            }
            if (head == nullptr)
                throw invalid_argument("input file does not contain data, cannot generate huffman code");

            tree_ptr->root = huffman_build(head);
            return tree_ptr;
        }
        else if (coding_type == "shannon")
        {
            if (freq_vec_stored.empty())
                freq_vec_stored = get_ordered_freq();

            char c;
            int f, depth, len = input_file->size(), alph = freq_vec_stored.size();
            for (int i = alph - 1; i > -1; i--)
            {
                c = freq_vec_stored[i].first;
                f = freq_vec_stored[i].second;
                depth = ceil(log2((double)len / f));
                tree_ptr->place(c, depth);
            }
            return tree_ptr;
        }
        else if (coding_type == "shannon-fano")
        {
            if (freq_vec_stored.empty())
                freq_vec_stored = get_ordered_freq();

            char c, bit;
            int f, kx, len = input_file->size();
            double prob, fx = 0, prob_sum = 0;

            for (int i = 0; i < (int)freq_vec_stored.size(); i++)
            {
                stringstream path;
                c = freq_vec_stored[i].first;
                f = freq_vec_stored[i].second;
                prob = (double)f / len;
                fx = prob_sum + prob / 2;
                kx = ceil(log2((double)len / f)) + 1;
                for (int k = 0; k < kx; k++)
                {
                    bit = ((fx *= 2) >= 1 ? '1' : '0');
                    fx -= (fx >= 1 ? 1 : 0);
                    path << bit;
                }
                prob_sum += prob;
                tree_ptr->place(path, c);
            }
            return tree_ptr;
        }
        else
        {invalid_argument("Unknown coding scheme");
         return tree_ptr;}
    }
};

data *decompress(data *&compressed_file, tree *&code_tree)
{
    stringstream comp_stream(compressed_file->get_content());
    stringstream decomp_stream;

    code_tree->decode(comp_stream, decomp_stream);
    data *file_ptr = new data(decomp_stream.str());
    return file_ptr;
}

class comp_decomp
{
private:
    string filename;
    tree *huffman_tree, *shannon_tree, *shanfano_tree;
    data *input_data, *comp_huff, *comp_shannon, *comp_shanfano, *decomp_huff, *decomp_shannon, *decomp_shanfano;

public:
    comp_decomp(string fn)
    {
        filename = fn;
    }
    ~comp_decomp()
    {
        delete huffman_tree;
        delete shannon_tree;
        delete shanfano_tree;
        delete input_data;
        delete comp_huff;
        delete comp_shannon;
        delete comp_shanfano;
        delete decomp_huff;
        delete decomp_shannon;
        delete decomp_shanfano;
    }

    void lights_camera_action()
    {
        cout << "Compressing file : " << filename << "    ......\n\n";
        input_data = new data;
        input_data->read_from_file(filename);

        compressor new_compressor(input_data);

        huffman_tree = new_compressor.compress("huffman");
        shannon_tree = new_compressor.compress("shannon");
        shanfano_tree = new_compressor.compress("shannon-fano");

        cout << "Huffman codes- \n";
        generate_comp_data(comp_huff, huffman_tree, input_data);
        cout << "Shannon Codes- \n";
        generate_comp_data(comp_shannon, shannon_tree, input_data);
        cout << "Shannon-Fano Codes - \n";
        generate_comp_data(comp_shanfano, shanfano_tree, input_data);

        comp_huff->write_to_binary_file("huff_comp.dat");
        comp_shannon->write_to_binary_file("shannon_comp.dat");
        comp_shanfano->write_to_binary_file("shanfano_comp.dat");

        comp_huff->write_to_text_file("huff_encoded_string.txt");
        comp_shannon->write_to_text_file("shannon_encoded_string.txt");
        comp_shanfano->write_to_text_file("shanfano_encoded_string.txt");

        decomp_huff = decompress(comp_huff, huffman_tree);
        decomp_shannon = decompress(comp_shannon, shannon_tree);
        decomp_shanfano = decompress(comp_shanfano, shanfano_tree);

        cout << "\nInput file size: " << input_data->size() << " bytes\n";
        cout << "Huffman compressed file size: " << comp_huff->size() << " bits\n";
        cout << "Shannon compressed file size: " << comp_shannon->size() << " bits\n";
        cout << "Shannon-Fano compressed file size: " << comp_shanfano->size() << " bits\n";

        cout << "Huffman decompressed file size: " << decomp_huff->size() << " bytes\n";
        cout << "Shannon decompressed file size: " << decomp_shannon->size() << " bytes\n";
        cout << "Shannon-Fano decompressed file size: " << decomp_shanfano->size() << " bytes\n\n";

        if (match(input_data, decomp_huff))
            cout << "Decompressed Huffman file matches with original file\n";
        if (match(input_data, decomp_shannon))
            cout << "Decompressed Shannon file matches with original file\n";
        if (match(input_data, decomp_shanfano))
            cout << "Decompressed Shannon-Fano file matches with original file\n";

        cout << "\nDONE"
             << "\n\n";
    }

    void generate_comp_data(data *&comp_data, tree *code_tree, data *input_file)
    {
        unordered_map<char, string> code_map = code_tree->give_codes();

        unordered_map<char, string>::iterator itr;
        for (itr = code_map.begin(); itr != code_map.end(); itr++)
        {
            cout << (itr->first) << " : " << (itr->second) << "\n";
        }
        string comp = "";
        for (char c : input_file->get_content())
        {
            comp += code_map[c];
        }
        comp_data = new data(comp);
    }

    bool match(data *&a, data *&b)
    {
        if (a->get_content() != b->get_content())
            return false;
        return true;
    }
};

int main(int argc, char **argv)
{

    comp_decomp zipper1(argv[1]);
    zipper1.lights_camera_action();

    return 0;
}
