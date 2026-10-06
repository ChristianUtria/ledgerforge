#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ledger.h"

struct Cuenta { int id; long long saldo; int ntrx; };

static std::string dinero(long long centavos) {
    std::ostringstream o;
    bool neg = centavos < 0;
    long long v = neg ? -centavos : centavos;
    std::string ent = std::to_string(v / 100);
    for (int i = (int)ent.size() - 3; i > 0; i -= 3) ent.insert(i, ",");
    o << (neg ? "-$" : "$") << ent << '.' << std::setw(2) << std::setfill('0') << v % 100;
    return o.str();
}

int main() {

    std::ifstream ft(LF_ARCHIVO_TRX, std::ios::binary);
    if (!ft) { std::cerr << "No existe " LF_ARCHIVO_TRX "\n"; return 1; }
    std::vector<uint8_t> datos((std::istreambuf_iterator<char>(ft)), std::istreambuf_iterator<char>());
    uint32_t calc = adler32_asm(datos.data(), datos.size());

    std::ifstream fs(LF_ARCHIVO_SUM);
    std::string hex; fs >> hex;
    uint32_t esperado = (uint32_t)std::stoul(hex, nullptr, 16);
    bool ok = (calc == esperado);
    std::printf("[C++]  Integridad (ASM Adler-32): %08x %s\n", calc, ok ? "OK" : "CORRUPTO");
    if (!ok) return 2;

    std::vector<Cuenta> cuentas;
    std::ifstream fsal(LF_ARCHIVO_SAL);
    for (std::string l; std::getline(fsal, l);) {
        if (l.size() < 23) continue;
        cuentas.push_back({std::stoi(l.substr(0, 6)), std::stoll(l.substr(6, 12)), std::stoi(l.substr(18, 5))});
    }

    std::map<int, long long> ref;
    std::ifstream ftx(LF_ARCHIVO_TRX);
    for (std::string l; std::getline(ftx, l);) {
        if (l.size() < LF_REG_LEN) continue;
        long long m = std::stoll(l.substr(7, 9));
        ref[std::stoi(l.substr(0, 6))] += (l[6] == 'C') ? m : -m;
    }
    int difs = 0;
    for (auto& c : cuentas) if (ref[c.id] != c.saldo) ++difs;
    bool coinciden = (difs == 0 && ref.size() == cuentas.size());
    std::printf("[C++]  Validacion cruzada COBOL vs C++: %s (%zu cuentas)\n",
                coinciden ? "COINCIDEN" : "DIFERENCIAS", cuentas.size());
    if (cuentas.empty()) return 3;

    std::sort(cuentas.begin(), cuentas.end(), [](const Cuenta& a, const Cuenta& b) { return a.saldo > b.saldo; });
    long long total = 0; for (auto& c : cuentas) total += c.saldo;
    long long mediana = cuentas[cuentas.size() / 2].saldo;
    auto negativas = std::count_if(cuentas.begin(), cuentas.end(), [](const Cuenta& c) { return c.saldo < 0; });

    std::cout << "\n===== REPORTE =====\n"
              << "Saldo total    : " << dinero(total) << "\n"
              << "Saldo promedio : " << dinero(total / (long long)cuentas.size()) << "\n"
              << "Mediana        : " << dinero(mediana) << "\n"
              << "Cuentas en rojo: " << negativas << " de " << cuentas.size() << "\n\n"
              << "Top 5 cuentas:\n";
    for (size_t i = 0; i < std::min<size_t>(5, cuentas.size()); ++i)
        std::cout << "  " << cuentas[i].id << "  " << std::setw(14) << dinero(cuentas[i].saldo)
                  << "  (" << cuentas[i].ntrx << " trx)\n";
    return coinciden ? 0 : 3;
}
