// AVL.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include <iostream>
using namespace std;

struct node {
    int data;
    int height;
    struct node* left;
    struct node* right;
};
typedef struct node AVLTREE;

AVLTREE* new_node(int data)
{
    AVLTREE* p = new AVLTREE;
    p->data = data;
    p->left = NULL;
    p->right = NULL;
    p->height = 1;
    return p;
}

AVLTREE* minimumValue(AVLTREE* root)
{
    AVLTREE* min = root; // root düğümü min e atanıyor

    while (min->left != NULL) // sol dugumu bos olmayan agactaki en kucuk degere sahip dugumu bul 
        min = min->left;     //min degerine ata

    return min;
}

int maximumValue(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int height(AVLTREE* h) //agacın yuksekligini bulma
{
    if (h == NULL) 
        return 0;
    return h->height;
}

AVLTREE* sagdon(AVLTREE* y)
{
    AVLTREE* cd = y->left; // y dügümünün sol cocugu cd ye atanıyor
    AVLTREE* dc = cd->right; // cd dügümünün sag cocugu dc ye atanıyor

    cd->right = y;  // y düğümü cd nin sag cocuğu olarak atanıyor
    y->left = dc;  // dc düğümü y nin sol cocuğu olarak atanıyor

    // dügüm yüksekligini güncelle
    y->height = maximumValue(height(y->left),height(y->right)) + 1;
    cd->height = maximumValue(height(cd->left),height(cd->right)) + 1;

    return cd;
}

AVLTREE* soldon(AVLTREE* x)
{
    AVLTREE* y = x->right; // x dügümünün sag cocugu y ye atanıyor
    AVLTREE* dc = y->left; // y dügümünün sol cocugu dc ye atanıyor

    y->left = x; // x düğümü y nin sol cocuğu olarak atanıyor
    x->right = dc; // dc düğümü x nin sag cocuğu olarak atanıyor

    // dügüm yüksekligini güncelle
    x->height = maximumValue(height(x->left),height(x->right)) + 1;
    y->height = maximumValue(height(y->left),height(y->right)) + 1;

    return y;
}

//sol alt agac ile sağ alt agac yuksekligi farkını hesaplama
int Balance(AVLTREE* b)
{
    if (b == NULL)
        return 0;
    return height(b->left) - height(b->right);
}

AVLTREE* insert(AVLTREE* root, int data)
{
    // 1. Agac bos ise
    if (root == NULL)
        return(new_node(data));
    //ekleyeceğimiz veri root'un data'sından küçükse
    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data) //ekleyeceğimiz veri root'un data'sından büyükse
        root->right = insert(root->right, data);
    else 
        return root;

    // root'un yüksekligini güncelle
    root->height = 1 + maximumValue(height(root->left),height(root->right));

    //Kokun dengesizligini kontrol etmek icin root'un balance degerini degiskene aktarma
    int balance = Balance(root);

    // Kokun dengesiz olma durumunda 

    // Sola dondurme   
    if (balance > 1 && data < root->left->data)
        return sagdon(root);

    // Saga dondurme 
    if (balance < -1 && data > root->right->data)
        return soldon(root);

    // Sola - Saga dondurme 
    if (balance > 1 && data > root->left->data)
    {
        root->left = soldon(root->left);
        return sagdon(root);
    }

    // Saga - Sola dondurme 
    if (balance < -1 && data < root->right->data)
    {
        root->right = sagdon(root->right);
        return soldon(root);
    }

    return root;
}

AVLTREE* delete_node(AVLTREE* root, int data)
{

    //Ağac yoksa
    if (root == NULL)
        return root;

    //ekleyeceğimiz veri root'un data'sından küçükse
    if (data < root->data)
        root->left = delete_node(root->left, data);

    //ekleyeceğimiz veri root'un data'sından büyükse 
    else if (data > root->data)
        root->right = delete_node(root->right, data);

    //Aranan düğümü bulduysak
    else
    {
        // tek cocugu olan veya cocuguolmayan  
        if ((root->left == NULL) || (root->right == NULL))
        {
            AVLTREE* temp = root->left ?
                root->left :
                root->right;

            // cocuk yoksa  
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // 1 cocugu varsa  
                *root = *temp; 
            free(temp);
        }
        else
        {
            AVLTREE* temp = minimumValue(root->right);
            root->data = temp->data;
            root->right = delete_node(root->right,temp->data);
        }
    }

    //bir dügüm varsa rootu dondur
    if (root == NULL)
        return root;

    // root'un yüksekligini güncelle  
    root->height = 1 + maximumValue(height(root->left),height(root->right));

    //Kokun dengesizligini kontrol etmek icin root'un balance degerini degiskene aktarma
    int balance = Balance(root);

    // Kokun dengesiz olma durumunda 

    // Sola dondurme   
    if (balance > 1 &&
        Balance(root->left) >= 0)
        return sagdon(root);

    // sol - sag dondurme 
    if (balance > 1 && Balance(root->left) < 0)
    {
        root->left = soldon(root->left);
        return sagdon(root);
    }

    // sag - sag dondurme   
    if (balance < -1 && Balance(root->right) <= 0)
        return soldon(root);

    // sag - sol dondurme
    if (balance < -1 && Balance(root->right) > 0)
    {
        root->right = sagdon(root->right);
        return soldon(root);
    }

    return root;
}

AVLTREE* searchTree(AVLTREE* tree, int data)
{
    if (tree != NULL) //Ağac doluysa (BST ile aynı islemler hmene hemen)
    {
        if (tree->data == data)
        {
            cout << endl;
            cout << "Sayi Bulundu : " << data << endl;
            return tree;
        }
        else if (tree->data > data)
        {
            searchTree(tree->left, data);
        }
        else
        {
            searchTree(tree->right, data);
        }
    }
    else//Ağac boş ise 
    {
        cout << endl;
        cout << "Aradiginiz sayi yok !.." << endl;
        return NULL;
    }
}

void preorder(AVLTREE* root)
{
    if (root != NULL) //Preorder sıralama
    {
        cout << root->data << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

int main()
{
    AVLTREE* cdroot = NULL;
    int i = 0;
    do
    {
        cout << "Agaca integer veri ekleyiniz..." << endl;
        cout << "Sayi : " << endl;
        cin >> i;
        if (i != -1)
            cdroot = insert(cdroot,i);
    } while (i != -1);

    cout << endl;
    cout << "Preorder: ";
    preorder(cdroot);
    cout << endl;

    cout << endl;
    cout << "Silinecek sayi: ";
    cin >> i;
    cdroot = delete_node(cdroot, i);
    cout << endl;
    cout << "Preorder: ";
    preorder(cdroot);

    cout << endl;
    cout << "Aranacak sayi: ";
    cin >> i;
    searchTree(cdroot, i);

    return 0;
}

// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
