/**
 * @file register.hpp
 *
 * メモリマップレジスタを読み書きする機能を提供する
 */

#pragma once

#include <cstddef>
#include <cstdint>

template<typename T>
struct ArrayLength {};

template<typename T, size_t N>
struct ArrayLength<T[N]> {
    static const size_t value = N;
};

/**
 * MemMapRegister はメモリマップドレジスタのラッパー
 *
 * MemMapRegister はユーザー プログラムに基礎となるレジスタのread/writeを強制します
 * 特定のビット幅、ビット幅はタイプから推定される
 * T::data T はテンプレートパラメータ、T::data は配列である必要がある
 */
template<typename T>
class MemMapRegister {
  public:
    T Read() const {
        T tmp;
        for (size_t i = 0; i < len_; ++i) {
            tmp.data[i] = value_.data[i];
        }
        return tmp;
    }

    void Write(const T& value) {
        for (size_t i = 0; i < len_; ++i) {
            value_.data[i] = value.data[i];
        }
    }

  private:
    volatile T value_;
    static const size_t len_ = ArrayLength<decltype(T::data)>::value;
};

template<typename T>
struct DefaultBitmap {
    T data[1];

    DefaultBitmap& operator=(const T& value) { data[0] = value; }
    operator T() const { return data[0]; }
};

/*
 * 設計: コンテナのようなクラス
 *
 * PortArray や DeviceContextArray などのコンテナのようなクラス
 * Size() メソッドと Iterator 型が必要
 * Size() は要素とイテレータの数を返す必要がある
 * その型のすべての要素を反復する必要がある
 *
 * 各要素には、要素の利用可能性を示すフラグが含まれる場合がある
 * たとえば、各ポートには「ポート有効/無効」ビットがある
 * Size() とイテレータは無効な要素をスキップしない
 */

template<typename T>
class ArrayWrapper {
  public:
    using ValueType = T;
    using Iterator = ValueType*;
    using ConstIterator = const ValueType*;

    ArrayWrapper(uintptr_t array_base_addr, size_t size)
        : array_(reinterpret_cast<ValueType*>(array_base_addr))
        , size_(size) {}

    size_t Size() const { return size_; }

    // begin, end, cbegin, cend must be lower case names
    // to be used in rage-based for statements.
    Iterator begin() { return array_; }
    Iterator end() { return array_ + size_; }
    ConstIterator cbegin() const { return array_; }
    ConstIterator cend() const { return array_ + size_; }

    ValueType& operator[](size_t index) { return array_[index]; }

  private:
    ValueType* const array_;
    const size_t size_;
};
