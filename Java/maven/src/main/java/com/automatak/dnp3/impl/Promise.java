package com.automatak.dnp3.impl;


interface Promise<T> {
    void set(T value);
}
