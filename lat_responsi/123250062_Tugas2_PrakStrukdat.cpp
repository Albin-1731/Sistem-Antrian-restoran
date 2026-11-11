
#include <iostream>
#include <string>
using namespace std;

struct Pelanggan {
    string nama;
    Pelanggan* next;
};
struct Queue {
    Pelanggan* front;
    Pelanggan* rear;
};
void inisialisasiQueue(Queue &q) {
    q.front = nullptr;
    q.rear = nullptr;
}
void enqueue(Queue &q, string nama) {
    Pelanggan* baru = new Pelanggan;
    baru->nama = nama;
    baru->next = nullptr;

    if (q.rear == nullptr) {
        q.front = baru;
        q.rear = baru;
    } else {
        q.rear->next = baru;
        q.rear = baru;
    }
}

void enqueueFront(Queue &q, string nama) {
    Pelanggan* baru = new Pelanggan;
    baru->nama = nama;
    baru->next = q.front;
    q.front = baru;
    if (q.rear == nullptr) {
        q.rear = baru;
    }
}
string dequeue(Queue &q) {
    if (q.front == nullptr) {
        return "";
    }
    Pelanggan* hapus = q.front;
    string nama = hapus->nama;
    q.front = q.front->next;

    if (q.front == nullptr) {
        q.rear = nullptr;
    }
    delete hapus;
    return nama;
}

bool hapusPelanggan(Queue &q, string nama) {
    Pelanggan* bantu = q.front;
    Pelanggan* prev = nullptr;

    while (bantu != nullptr) {
        if (bantu->nama == nama) {

            if (prev == nullptr) {
                q.front = bantu->next;
            } else {
                prev->next = bantu->next;
            }
            if (q.rear == bantu) {
                q.rear = prev;
            }
            delete bantu;
            return true;
        }
        prev = bantu;
        bantu = bantu->next;
    }
    return false;
}
void tampilQueue(Queue q) {
    if (q.front == nullptr) {
        cout << "Antrian kosong\n";
        return;
    }
    Pelanggan* bantu = q.front;
    while (bantu != nullptr) {
        cout << bantu->nama << " menunggu menu\n";
        bantu = bantu->next;
    }
}
struct Node {
    string nama;
    string kategori;
    int harga;
    bool tersedia;
    Queue antrian;
    Node* left;
    Node* right;
};

Node* buatNode(string nama, string kategori, int harga) {
    Node* baru = new Node;
    baru->nama = nama;
    baru->kategori = kategori;
    baru->harga = harga;
    baru->tersedia = true;

    inisialisasiQueue(baru->antrian);

    baru->left = nullptr;
    baru->right = nullptr;

    return baru;
}

Node* tambahMenu(Node* root, string nama,
                 string kategori, int harga) {

    if (root == nullptr) {
        return buatNode(nama, kategori, harga);
    }

    if (nama < root->nama) {
        root->left = tambahMenu(
            root->left, nama, kategori, harga);
    }
    else if (nama > root->nama) {
        root->right = tambahMenu(
            root->right, nama, kategori, harga);
    }
    else {
        cout << "Menu sudah ada!\n";
    }

    return root;
}
Node* cariMenu(Node* root, string nama) {
    if (root == nullptr || root->nama == nama) {
        return root;
    }
    if (nama < root->nama) {
        return cariMenu(root->left, nama);
    }
    return cariMenu(root->right, nama);
}
void tampilMenu(Node* root) {
    if (root == nullptr) {
        return;
    }
    tampilMenu(root->left);

    cout << "\nNama     : " << root->nama;
    cout << "\nKategori : " << root->kategori;
    cout << "\nHarga    : Rp" << root->harga;
    cout << "\nStatus   : " << (root->tersedia ? "Tersedia" : "Habis");
    cout << "\n-------------------------\n";
    tampilMenu(root->right);
}

void hapusQueue(Queue &q) {
    while (q.front != nullptr) {
        dequeue(q);
    }
}
Node* hapusMenu(Node* root, string nama) {
    if (root == nullptr) {
        return nullptr;
    }

    if (nama < root->nama) {
        root->left = hapusMenu(root->left, nama);
    }
    else if (nama > root->nama) {
        root->right = hapusMenu(root->right, nama);
    }
    else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            hapusQueue(root->antrian);
            delete root;
            return temp;
        }

        if (root->right == nullptr) {
            Node* temp = root->left;
            hapusQueue(root->antrian);
            delete root;
            return temp;
        }
        Node* penerus = root->right;

        while (penerus->left != nullptr) {
            penerus = penerus->left;
        }

        root->nama = penerus->nama;
        root->kategori = penerus->kategori;
        root->harga = penerus->harga;
        root->tersedia = penerus->tersedia;

        hapusQueue(root->antrian);

        while (penerus->antrian.front != nullptr) {
            enqueue(root->antrian,
                    dequeue(penerus->antrian));
        }

        root->right = hapusMenu(
            root->right, penerus->nama);
    }
    return root;
}

struct history {
    int jenis;
    string namaMenu;
    string namaPelanggan;
    bool masukAntrian;
    string pelangganDilayani;

    history* next;
};

history* top = nullptr;
void push(int jenis, string menu,
          string pelanggan = "",
          bool antrian = false,
          string dilayani = "") {

    history* baru = new history;

    baru->jenis = jenis;
    baru->namaMenu = menu;
    baru->namaPelanggan = pelanggan;
    baru->masukAntrian = antrian;
    baru->pelangganDilayani = dilayani;

    baru->next = top;
    top = baru;
}

// Tambah menu
void menambahMenu(Node* &root) {
    string nama, kategori;
    int harga;

    cout << "Nama menu     : ";
    getline(cin, nama);

    if (cariMenu(root, nama) != nullptr) {
        cout << "Menu sudah ada!\n";
        return;
    }

    cout << "Kategori      : ";
    getline(cin, kategori);

    cout << "Harga         : ";
    cin >> harga;
    cin.ignore();

    root = tambahMenu(root, nama, kategori, harga);

    cout << "Menu berhasil ditambahkan\n";
}

// lihat menu
void LihatMenu(Node* root) {
    if (root == nullptr) {
        cout << "Belum ada menu\n";
        return;
    }

    tampilMenu(root);
}

// catat pesanan
void Pesanan(Node* root) {
    string pelanggan, namaMenu;

    cout << "Nama pelanggan : ";
    getline(cin, pelanggan);

    cout << "Nama menu      : ";
    getline(cin, namaMenu);

    Node* menu = cariMenu(root, namaMenu);

    if (menu == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (menu->tersedia) {
        push(1, namaMenu, pelanggan, false);

        cout << "Pesanan " << pelanggan<< " berhasil dibuat\n";
    }
    else {
        push(1, namaMenu, pelanggan, true);
        enqueue(menu->antrian, pelanggan);
        cout << "Menu sedang habis, "<< pelanggan<< " masuk ke dalam waiting list\n";
    }
}

// perbarui status menu
void UpdateMenu(Node* root) {
    string nama;

    cout << "Nama menu: ";
    getline(cin, nama);
    Node* menu = cariMenu(root, nama);

    if (menu == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (menu->tersedia) {
        cout << "Menu sudah tersedia\n";
        return;
    }

    string dilayani = "";

    menu->tersedia = true;

    // ambil satu pelanggan dari antrean
    if (menu->antrian.front != nullptr) {
        dilayani = dequeue(menu->antrian);
    }

    push(2, nama, "", false, dilayani);

    cout << "Menu tersedia kembali\n";

    if (dilayani != "") {
        cout << "Pesanan langsung diberikan kepada "<<dilayani<< " dari waiting list\n";
    }
}
// 5. hapus menu
void HapusMenu(Node* &root) {
    string nama;

    cout << "Nama menu yang dihapus: ";
    getline(cin, nama);

    if (cariMenu(root, nama) == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    root = hapusMenu(root, nama);

    cout << "Menu berhasil dihapus\n";
}

// 6. Undo 
void Undo(Node* root) {
    if (top == nullptr) {
        cout << "Tidak ada history untuk di-undo\n";
        return;
    }

    history* hapus = top;
    top = top->next;

    Node* menu = cariMenu(root, hapus->namaMenu);

    if (menu == nullptr) {
        cout << "Menu sudah tidak ditemukan\n";
        delete hapus;
        return;
    }

    if (hapus->jenis == 1) {

        // Undo pesanan
        if (hapus->masukAntrian) {
            hapusPelanggan(
                menu->antrian,
                hapus->namaPelanggan
            );
        }
        cout << "Undo: Pesanan menu "
             << hapus->namaMenu<< " dibatalkan\n";
    }
    else if (hapus->jenis == 2) {

        // Undo pembaruan status
        menu->tersedia = false;

        // Mengembalikan pelanggan ke depan antrean
        if (hapus->pelangganDilayani != "") {
            enqueueFront(
                menu->antrian,
                hapus->pelangganDilayani
            );
        }

        cout << "Undo: Menu "<<hapus->namaMenu<< " kembali habis\n";
    }
    delete hapus;
}

// lihat waiting list
void WaitingList(Node* root) {
    string nama;
    cout << "Nama menu: ";
    getline(cin, nama);
    
    Node* menu = cariMenu(root, nama);

    if (menu == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }
    if (menu->antrian.front == nullptr) {
        cout << "Antrian kosong\n";
        return;
    }
    Pelanggan* bantu = menu->antrian.front;
    while (bantu != nullptr) {
        cout << bantu->nama<< " menunggu menu "<< menu->nama << endl;
        bantu = bantu->next;
    }
}

int main() {
    Node* root = nullptr;
    int pilihan;
    do {
        system("cls");
        cout << "\n================================\n";
        cout << "     SISTEM PEMESANAN RESTORAN\n";
        cout << "================================\n";
        cout << "1. Tambah Menu\n";
        cout << "2. Lihat Daftar Menu\n";
        cout << "3. Pesan Menu\n";
        cout << "4. Menu Tersedia Kembali\n";
        cout << "5. Hapus Menu\n";
        cout << "6. Undo Aktivitas\n";
        cout << "7. Lihat Waiting List\n";
        cout << "8. Keluar\n";
        cout << "================================\n";
        cout << "Pilih menu: ";

        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                menambahMenu(root);break;
            case 2:
                LihatMenu(root);break;
            case 3:
                Pesanan(root);break;
            case 4:
                UpdateMenu(root);break;
            case 5:
                HapusMenu(root);break;
            case 6:
                Undo(root);break;
            case 7:
                WaitingList(root);break;
            case 8:
                cout << "Terima kasih!\n";break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        if (pilihan != 0) {
            cout << "\nTekan Enter untuk kembali ke menu...";
            cin.ignore(1000, '\n');
            cin.get();
        }
    } while (pilihan != 8);
    return 0;
}