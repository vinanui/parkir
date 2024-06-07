#include <iostream>
#include <cstring>
#include <time.h>
#include <stack>
#include <conio.h> // untuk getch()

using namespace std;

/*
Struct
*/

// Struktur data untuk Akun
struct Akun {
  string username;
  string password;
  int level; // 0: admin, 1: user
};

// Struktur data untuk kendaraan
struct Kendaraan {
  char platNomor[15];
  int jenisKendaraan; //0: Motor, 1: Mobil Kecil, 2: Mobil Besar
  string username;
  time_t waktuMasuk;
  time_t waktuKeluar;
};

// Struktur data untuk daftar kendaraan
struct ListKendaraan {
  Kendaraan data;
  ListKendaraan* sebelumnya;
  ListKendaraan* berikutnya;
};

/*
Global Variable
*/

ListKendaraan* head = NULL; // Head List Kendaraan
stack<Kendaraan> riwayat; // Tumpukan riwayat kendaraan

Akun akun[5] = {
        {"admin", "123456", 0}, // Akun admin dengan level 0
        {"user", "132456", 1}, // Akun user dengan level 1
        {"user2", "132456", 1}  // Akun user dengan level 1
    };

/*
Utils
*/

void clearAndPause(bool pause = false) {
    if (pause)
    {
        system("pause");
    }
    system("cls");  // Clear screen
}

// Fungsi untuk menghitung lama parkir
int hitungLamaParkir(time_t waktuMasuk, time_t waktuKeluar) {
  return difftime(waktuKeluar, waktuMasuk);
}

// Fungsi untuk menghitung biaya parkir
int hitungBiayaParkir(int jenisKendaraan, int lamaParkir) {
  int biaya = 0;

  switch (jenisKendaraan) {
    case 1: // Motor
      biaya = 2000 * (lamaParkir/3600);
      break;
    case 2: // Mobil Kecil
      biaya = 3000 * (lamaParkir/3600);
      break;
    case 3: // Mobil Besar
      biaya = 5000 * (lamaParkir/3600);
      break;
  }

  return biaya;
}

// Fungsi untuk menambahkan riwayat ke stack
void pushRiwayat(Kendaraan data) {
  riwayat.push(data);
}

// Fungsi baru untuk menampilkan informasi kendaraan
void tampilkanInfoKendaraan(const Kendaraan& data) {
  cout << data.platNomor << "\t";
  switch (data.jenisKendaraan) {
    case 1:
      cout << "Motor\t\t";
      break;
    case 2:
      cout << "Mobil Kecil\t\t";
      break;
    case 3:
      cout << "Mobil Besar\t\t";
      break;
  }
  cout << asctime(localtime(&data.waktuMasuk)) << "\t";
  if (&data.waktuKeluar)
  {
    cout << asctime(localtime(&data.waktuKeluar)) << endl;
  }
  
}

/*
Features
*/

// Fungsi untuk menambahkan kendaraan ke daftar List Kendaraan
void tambahKendaraan(Kendaraan kendaraan) {
  ListKendaraan* baru = new ListKendaraan;
  baru->data = kendaraan;
  baru->berikutnya = head;
  baru->sebelumnya = NULL;

  if (head) {
    head->sebelumnya = baru;
  }

  head = baru;
}

// Fungsi untuk menghapus kendaraan dari daftar List Kendaraan
void hapusKendaraan(char* platNomor) {
  ListKendaraan* curr = head;

  while (curr && strcmp(curr->data.platNomor, platNomor) != 0) {
    curr = curr->berikutnya;
  }

  if (curr) {
    if (curr == head) {
      head = curr->berikutnya;
    } else {
      curr->sebelumnya->berikutnya = curr->berikutnya;
    }

    if (curr->berikutnya) {
      curr->berikutnya->sebelumnya = curr->sebelumnya;
    }

    delete curr;
  }
}

// Fungsi untuk mencari kendaraan berdasarkan plat nomor
ListKendaraan* cariKendaraan(char* platNomor) {
  ListKendaraan* curr = head;

  while (curr) {
    if (strcmp(curr->data.platNomor, platNomor) == 0) {
      return curr; // Jika plat nomor ditemukan, kembalikan pointer ke node
    }
    curr = curr->berikutnya;
  }

  return NULL; // Jika kendaraan tidak ditemukan, kembalikan NULL
}

// Fungsi untuk parkir kendaraan
void parkirKendaraan(int indexAkun) {
  Kendaraan kendaraan;

  cout << "\n**Parkir Kendaraan**\n";
  cout << "Plat nomor: ";
  cin >> kendaraan.platNomor;
  cout << "Jenis kendaraan (1: Motor, 2: Mobil Kecil, 3: Mobil Besar): ";
  cin >> kendaraan.jenisKendaraan;

  kendaraan.username = akun[indexAkun].username;
  kendaraan.waktuMasuk = time(NULL);

  tambahKendaraan(kendaraan);

  cout << "Kendaraan dengan plat nomor " << kendaraan.platNomor << " telah diparkir." << endl;
}

// Fungsi untuk keluar parkir
void keluarParkir(int indexAkun) {
  char platNomor[15];

  cout << "\n**Keluar Parkir**\n";
  cout << "Plat nomor: ";
  cin >> platNomor;

  ListKendaraan* kendaraan;
  kendaraan = cariKendaraan(platNomor);
  if (kendaraan != NULL)
  {
    kendaraan->data.waktuKeluar = time(NULL);
    int lamaParkir = hitungLamaParkir(kendaraan->data.waktuMasuk, kendaraan->data.waktuKeluar);
    int biaya = hitungBiayaParkir(kendaraan->data.jenisKendaraan, lamaParkir);

    cout << "Kendaraan dengan plat nomor " << platNomor << " telah keluar." << endl;
    cout << "Lama parkir: " << lamaParkir / 3600 << " jam" << endl;
    cout << "Biaya parkir: Rp" << biaya << "." << endl;
    
    pushRiwayat(kendaraan->data);
    hapusKendaraan(platNomor);
    return;
  }
  
  cout << "Plat nomor tidak ditemukan." << endl;
  return;
}

// Fungsi untuk melihat daftar kendaraan terparkir
void daftarKendaraanTerparkir() {
  ListKendaraan* curr = head;

  if (head == NULL) {
    cout << "Tidak ada kendaraan yang terparkir.\n";
  } else {
    cout << "\n**Daftar Kendaraan Terparkir**\n";
    cout << "Plat Nomor\tJenis Kendaraan\tWaktu Masuk\n";
    while (curr) {
      tampilkanInfoKendaraan(curr->data);
      curr = curr->berikutnya;
    }
  }
}

// Fungsi untuk menampilkan stack riwayat parkir
void tampilkanRiwayat(string username = "") {
  stack<Kendaraan> temp = riwayat;
  if (temp.empty()) {
    cout << "Riwayat parkir kosong." << endl;
    return;
  }

  cout << "\n**Riwayat Parkir**" << endl;
  if (username != "") {
    cout << "Username: " << username << endl;
  }

  cout << "Plat Nomor\tJenis Kendaraan\tWaktu Masuk\tWaktu Keluar\n";
  while (!temp.empty()) {
    Kendaraan data = temp.top();
    if (username != "") {
      // Menampilkan data kendaraan hanya jika username cocok
      if (data.username == username) {
        tampilkanInfoKendaraan(data); // Fungsi baru untuk menampilkan info kendaraan
        cout << endl;
      }
    } else {
      // Menampilkan data kendaraan untuk semua username
      tampilkanInfoKendaraan(data); // Fungsi baru untuk menampilkan info kendaraan
      cout << endl;
    }
    
    temp.pop();
  }
}

// Fungsi untuk login
int login(){
    string username, password = "";

    clearAndPause();
    cout<<"Username : "; 
    cin>>username;
    char ch;
    cout << "Password : ";
    ch = _getch();
    while (ch != 13) { // 13 adalah kode ASCII untuk tombol Enter
        if (ch == '\b') { // 8 adalah kode ASCII untuk tombol Backspace
            if (password.length() > 0) {
                password.pop_back();
                cout << "\b \b"; // Menghapus karakter dari layar
            }
        } else {
            password.push_back(ch);
            cout << "*"; // Menampilkan karakter bintang sebagai ganti
        }
        ch = _getch();
    }
    cout << endl;

    // Loop untuk mengecek setiap akun
    for (int i = 0; i < 5; i++) {
        if (akun[i].username == username && akun[i].password == password) {
            cout << "Login Berhasil" << endl;
            clearAndPause(true);
            return i; // Mengembalikan nilai
        }
    }

    cout << "Login Gagal" << endl;
    return -1;
}

// Fungsi untuk menampilkan menu - Admin
void displayMenuAdmin() {
    cout << "============ ADMIN ============" << endl;
    cout << "1. Pembayaran" << endl;
    cout << "2. Histori" << endl;
    cout << "3. Monitoring Kepadatan" << endl;
    cout << "4. List Kendaaran" << endl;
    cout << "5. Cari Kendaraan" << endl;
    cout << "6. Keluar" << endl;
}

// Fungsi untuk menampilkan menu - User
void displayMenuUser() {
    cout << "============ USER ============" << endl;
    cout << "1. Parkir Kendaraan" << endl;
    cout << "2. Histori Pribadi" << endl;
    cout << "3. Monitoring Kepadatan" << endl;
    cout << "4. Keluar" << endl;
}

// Fungsi untuk aplikasi bagian admin
void menuAdmin(int indexAkun) {
    int choice;
    char platNomor[15];
    ListKendaraan* hasilPencarian = NULL; // Deklarasi di luar blok switch-case

    while (choice != 6)
    {
        displayMenuAdmin();
        cout << "Pilihan (1-6): ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Pilihan tidak valid. Silakan masukkan angka antara 1 dan 6." << endl;
            clearAndPause(true);
            continue;
        }
        
        switch (choice)
        {
        case 1:
            keluarParkir(indexAkun);
            clearAndPause(true);
            continue;
        case 2:
            tampilkanRiwayat();
            clearAndPause(true);
            continue;
        case 4:
            daftarKendaraanTerparkir();
            clearAndPause(true);
            continue;
        case 5:
            cout << "Masukkan plat nomor kendaraan yang dicari: ";
            cin >> platNomor;
            hasilPencarian = cariKendaraan(platNomor);
            if (hasilPencarian) {
                cout << "\nKendaraan ditemukan:\n";
                cout << "Plat Nomor\tJenis Kendaraan\tWaktu Masuk\n";
                tampilkanInfoKendaraan(hasilPencarian->data);
            } else {
                cout << "Kendaraan dengan plat nomor " << platNomor << " tidak ditemukan." << endl;
            }
            clearAndPause(true);
            continue;
        case 6:
            cout << "Sampai jumpa kembali " << akun[indexAkun].username << "!" << endl;
            clearAndPause(true);
            break;
        default:
            cout << "Pilihan tidak valid. Silakan pilih menu yang tersedia." << endl;
            clearAndPause(true);
            continue;
        }
    }
}

// Fungsi untuk aplikasi bagian user
void menuUser(int indexAkun) {
    int choice;
    while (choice != 4)
    {
        displayMenuUser();
        cout << "Pilihan (1-4): ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Pilihan tidak valid. Silakan masukkan angka antara 1 dan 4." << endl;
            clearAndPause(true);
            continue;
        }
        
        switch (choice)
        {
        case 1:
            parkirKendaraan(indexAkun);
            clearAndPause(true);
            continue;
        case 2:
            tampilkanRiwayat(akun[indexAkun].username);
            clearAndPause(true);
            continue;
        case 4:
            cout << "Sampai jumpa kembali " << akun[indexAkun].username << "!" << endl;
            clearAndPause(true);
            break;
        default:
            cout << "Pilihan tidak valid. Silakan pilih menu yang tersedia." << endl;
            clearAndPause(true);
            continue;
        }
    }
}

int main() {
    int loggedIn, choice;

    while (choice != 2)
    {
        cout << "Pilih Menu:" << endl;
        cout << "[1] Login" << endl;
        cout << "[2] Keluar Apps" << endl;
        cout << "Pilihan (1-2): " ;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Pilihan tidak valid. Silakan masukkan angka antara 1 dan 2." << endl;
            clearAndPause(true);
            continue;
        }

        switch (choice)
        {
        case 1:
            loggedIn = login();
            if (loggedIn == -1)
            {
                clearAndPause(true);
                continue;
            }
            
            switch (akun[loggedIn].level)
            {
            case 0:
                menuAdmin(loggedIn);
                clearAndPause();
                continue;
            case 1:
                menuUser(loggedIn);
                clearAndPause();
                continue;
            }

        case 2:
            // Keluar dari program
            cout << "Terima kasih telah menggunakan aplikasi kami." << endl;
            clearAndPause(true);
            break;
    
        default:
            cout << "Pilihan tidak valid. Silakan pilih menu yang tersedia." << endl;
            clearAndPause(true);
            break;
        }
    }
    
    return 0;
}
