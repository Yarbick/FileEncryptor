#pragma once

#include <string>
#include <cstdint>


/// Расширение для зашифрованных файлов (File ENCrypted)
inline const std::string ENCRYPTED_FILE_EXTENSION = "fenc";

/// Модуль для хешей
constexpr uint64_t HASH_MOD = 18446744073709551557ull;
/// Количество объединяемых байтов в блок-хеш
constexpr uint64_t BLOCK_SIZE = 4;
/// Лимит hashBase хеша
constexpr uint64_t MIN_HASH_BASE = 256, MAX_HASH_BASE = 65536 - MIN_HASH_BASE - 1; // Максимум с запасом для >= 256
/// Лимит subCoef хеша
constexpr uint64_t MIN_SUB_COEF = 1000, MAX_SUB_COEF = HASH_MOD - MIN_SUB_COEF - 1; // Максимум с запасом для >= 1000
