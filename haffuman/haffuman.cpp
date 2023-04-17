#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <queue>

#define ROW 50
using namespace std;
class tree
{ // haffuman树的节点
public:
    string key;  //赫夫曼树记录的数据
    int weight;  //权重
    string code; //赫夫曼编码
    tree *right;
    tree *left;
    tree *parent;
};
class List
{ //这是未生成树之前的节点，也可以叫做森林
public:
    tree onenote;
    shared_ptr<List> next;
};
typedef tree *trew;
typedef List *link;
typedef shared_ptr<List> lists;
void init_haffuman_list(lists head, vector<string> key, vector<int> weight);        //生成初始化赫夫曼森林
void display(lists head);                                                           //打印森林
lists get_min(lists head);                                                          //如其名
void skip(lists &head, lists target);                                               //删除元素
void newtree(lists &head, lists &rubbsh);                                           //更新森林
trew creat_haffuman(lists &head, lists &rubbsh);                                    //创造赫夫曼树
void traversalTree(trew root, unordered_map<string, string> &haffuman);             //遍历赫夫曼树并生成编码
void init_map(unordered_map<string, string> &haffuman);                             //如其名
fstream generate_code(string target, unordered_map<string, string> haffuman_code);  //将赫夫曼编码存入文件
fstream decipher_code(unordered_map<string, string> haffuman_code, string &result); //解码，并存入文件
void Print();                                                                       //数据的赫夫曼编码的打印
void Tree_print();                                                                  //树的形象化打印
void init_Tree_print(trew root);                                                    //树的形象化并存入文件
void get_high(trew root, int current_high, int &high);                              //得到赫夫曼树的高
int pow(int a, int b);                                                              //如其名

int main()
{
    // initialization
    int n;
    fstream hfm("hfmTree");
    if (!hfm)
        exit(-1);
    lists head = make_shared<List>(); //森林的头结点，后面的节点可以去除，所以用智能指针
    head->next = nullptr;
    lists rubbush = make_shared<List>();
    unordered_map<string, string> haffuman_code;
    trew root = nullptr; //树的根节点
    string judge;        //作为一系列输入的判断标志
    cout << "是否重新构造Haffuman树:yes or no";
    cin >> judge;
    if (judge == "yes")
    {
        ofstream close("hfmTree", ofstream::out);
        close.close();
        cout << "输入数的节点数量：";
        cin >> n;
        vector<string> key(n, "0");
        vector<int> weight(n, 0);
        for (int i = 0; i < n; i++) //输入一系列参数
        {
            getchar();
            cout << "输出每个节点的key:";
            getline(cin, key[i]);
            cout << "请输入权重值：";
            cin >> weight[i];
        }
        // creat haffuman tree
        init_haffuman_list(head, key, weight);
        root = creat_haffuman(head, rubbush);
        display(head); //打印一系列数据
        traversalTree(root, haffuman_code);
        for (const auto &w : haffuman_code)
        {
            cout << w.first << " : " << w.second << endl;
            hfm << w.first << " : " << w.second << endl;
        }
        init_Tree_print(root);
        Tree_print();
    }
    else
    {
        string name;
        //直接通过读取文件生成map
        init_map(haffuman_code);
    }
    // Encoding
    string target;
    cout << "请输入需要编码的字符：";
    getchar();
    getline(cin, target);
    generate_code(target, haffuman_code);
    cout << "是否需要解码: yes or no";
    cin >> judge;
    if (judge == "yes")
    {
        string result;
        decipher_code(haffuman_code, result);
        cout << "the real words is:" << result << endl;
    }
    Print();
    cout << endl;
    cout << "是否查看赫夫曼树:yes/no" << endl;
    cin >> judge;
    if (judge == "yes")
        Tree_print();
    system("pause");
    return 0;
}
void init_haffuman_list(lists head, vector<string> key, vector<int> weight)
{
    head->onenote.key = key[0];
    head->onenote.weight = weight[0];
    lists current = head;
    for (int i = 1; i < key.size(); i++)
    {
        lists new_note = make_shared<List>();
        new_note->onenote.key = key[i];
        new_note->onenote.weight = weight[i];
        new_note->onenote.left = nullptr;
        new_note->onenote.right = nullptr;
        new_note->onenote.parent = nullptr;
        new_note->next = nullptr;
        current->next = new_note;
        current = current->next;
    }
}
void display(lists head)
{
    while (head != nullptr)
    {
        cout << head->onenote.key << " ";
        head = head->next;
    }
    cout << endl;
}
lists get_min(lists head)
{
    lists min = head;
    head = head->next;
    while (head != nullptr)
    {
        if (head->onenote.weight <= min->onenote.weight)
        {
            min = head;
        }
        head = head->next;
    }
    return min;
}
void skip(lists &head, lists target)
{
    if (target == head)
    {
        head = head->next;
        return;
    }
    lists pre = head, current = head->next;
    while (current != nullptr)
    {
        if (current == target)
        {
            pre->next = current->next;
            return;
        }
        pre = pre->next;
        current = current->next;
    }
}

void newtree(lists &head, lists &rubbsh)
{
    lists min1 = get_min(head);
    skip(head, min1);
    lists min2 = get_min(head);
    skip(head, min2);
    lists sum = make_shared<List>();
    sum->onenote.left = &(min1->onenote);
    sum->onenote.right = &(min2->onenote);
    min1->onenote.parent = &(sum->onenote);
    min2->onenote.parent = &(sum->onenote);
    sum->onenote.weight = min1->onenote.weight + min2->onenote.weight;
    sum->onenote.key = min1->onenote.key + min2->onenote.key;
    sum->next = head;
    head = sum;
    min1->next = min2;
    min2->next = rubbsh;
    rubbsh = min1;
}
trew creat_haffuman(lists &head, lists &rubbsh)
{
    while (head->next != nullptr)
    {
        newtree(head, rubbsh);
        // display(head);
    }
    return &(head->onenote);
}
void traversalTree(trew root, unordered_map<string, string> &haffuman_code)
{
    if (root != nullptr)
    {
        if (root->left != nullptr)
            root->left->code = root->code + "0";
        if (root->right != nullptr)
            root->right->code = root->code + "1";
        traversalTree(root->left, haffuman_code);
        if (root->right == nullptr && root->left == nullptr)
            haffuman_code.emplace(root->key, root->code);
        traversalTree(root->right, haffuman_code);
    }
}
void init_map(unordered_map<string, string> &haffuman)
{
    fstream hfm("hfmTree");
    if (!hfm)
        exit(-1);
    string line, key, code;
    getline(hfm, line);
    int begin, end, current;
    while (line != "")
    {
        begin = 0;
        end = 0;
        current = 0;
        while (line[current] != ' ')
        {
            end++;
            current++;
        }
        key = line.substr(begin, end);
        code = line.substr(end + 3);
        haffuman.emplace(key, code);
        getline(hfm, line);
    }
    hfm.close();
}
fstream generate_code(string target, unordered_map<string, string> haffuman_code)
{
    ofstream close_codefile("CodeFile", ofstream::out);
    if (!close_codefile)
        exit(-1);
    close_codefile.close();
    fstream codefile("CodeFile");
    int n = target.size();
    for (int i = 0; i < n; i++)
    {
        codefile << haffuman_code[target.substr(i, 1)];
    }
    // codefile<<endl;
    string name;
    codefile.close();
    codefile.open("CodeFile");
    codefile >> name;
    // cout<<name;
    return codefile;
}
fstream decipher_code(unordered_map<string, string> haffuman_code, string &result)
{
    fstream codefile("CodeFile");
    string code;
    codefile >> code;
    codefile.close();
    ofstream close("TextFile", ofstream::out);
    close.close();
    fstream textfile("TextFile");
    int n = code.size();
    int begin = 0, end = 1;
    unordered_map<string, string> haffuman_decode;
    for (const auto &w : haffuman_code)
    {
        haffuman_decode.emplace(w.second, w.first);
    }
    for (end; end <= n; end++)
    {
        if (haffuman_decode.find(code.substr(begin, end - begin)) == haffuman_decode.end())
        {
            continue;
        }
        else
        {
            result.append(haffuman_decode[code.substr(begin, end - begin)]);
            begin = end;
        }
    }
    textfile << result;
    return codefile;
}
void Print()
{
    ofstream close("CodePrin");
    close.close();
    fstream codefile("CodeFile");
    fstream codeprin("CodePrin");
    string code;
    getline(codefile, code);
    int num = 0;
    cout << endl;
    cout << "CodeFile文件中的内容为:" << endl;
    for (auto word : code)
    {
        cout << word;
        codeprin << word;
        num++;
        if (num % ROW == 0)
        {
            cout << endl;
            codeprin << endl;
        }
    }
}
void init_Tree_print(trew root)
{
    ofstream close("TreePrin", ofstream::out);
    close.close();
    fstream treeprin("TreePrin");
    int high = 0;
    get_high(root, 0, high);
    high++;
    vector<vector<string>> matrix(high);
    for (int i = 0; i < high; i++)
        matrix[i].resize(pow(2, i));
    queue<trew> ques;
    ques.push(root);
    int row = 0, column = 0;
    trew current = root;
    int lows = 0;
    while (row < high)
    {
        current = ques.front();
        if (current == nullptr)
        {
            ques.push(nullptr);
            ques.push(nullptr);
            matrix[row][column] = " ";
            column++;
            if (column == matrix[row].size())
            {
                column = 0;
                row++;
            }
        }
        else
        {
            if (current->left == nullptr && current->right == nullptr)
                matrix[row][column] = current->key;
            else
                matrix[row][column] = "*";
            column++;
            if (column == matrix[row].size())
            {
                column = 0;
                row++;
            }
            ques.push(current->left);
            ques.push(current->right);
        }
        ques.pop();
    }
    for (int i = 0; i < high; i++)
    {
        treeprin << " ";
        for (int k = 0; k < matrix[i].size(); k++)
        {
            for (int j = 0; j < pow(2, high - i - 2); j++)
                treeprin << " ";
            if(matrix[i][k]!=" "){
                for (int j = 0; j < pow(2, high - i - 2) - 1; j++)
                    treeprin << "_";
            }
            else{
                for (int j = 0; j < pow(2, high - i - 2) - 1; j++)
                    treeprin << " ";
            }
            treeprin << matrix[i][k];
            if(matrix[i][k]!=" "){
                for (int j = 0; j < pow(2, high - i - 2) - 1; j++)
                    treeprin << "_";
            }
            else{
                for (int j = 0; j < pow(2, high - i - 2) - 1; j++)
                    treeprin << " ";
            }
            for (int j = 0; j < pow(2, high - i - 2) + 1; j++)
                treeprin << " ";
        }
        treeprin << endl;
        for (int k = 0; k < matrix[i].size(); k++)
        {
            treeprin << " ";
            if (i != high - 1 && matrix[i][k] != " ")
            {
                for (int j = 0; j < pow(2, high - i - 2) - 1; j++)
                    treeprin << " ";
                treeprin << "/";
                for (int j = 0; j < pow(2, high - i - 1) - 1; j++)
                    treeprin << " ";
                treeprin << "\\";
                for (int j = 0; j < pow(2, high - i - 2) - 1; j++)
                    treeprin << " ";
            }
            else if (matrix[i][k] == " ")
            {
                for (int j = 0; j < pow(2, high - i) - 1; j++)
                    treeprin << " ";
            }
        }
        treeprin << endl;
    }
}

void get_high(trew root, int current_high, int &max_high)
{
    if (root != nullptr)
    {
        if (root->left == nullptr && root->right == nullptr && current_high > max_high)
            max_high = current_high;
        current_high++;
        get_high(root->left, current_high, max_high);
        get_high(root->right, current_high, max_high);
    }
}
int pow(int a, int b)
{
    if (b < 0)
        return 0;
    else if (b == 0)
        return 1;
    else
        return a * pow(a, b - 1);
}
void Tree_print()
{
    cout << "the tree is like:" << endl;
    fstream treeprin("TreePrin");
    string out;
    getline(treeprin, out);
    while (out != "")
    {
        cout << out << endl;
        getline(treeprin, out);
    }
}
