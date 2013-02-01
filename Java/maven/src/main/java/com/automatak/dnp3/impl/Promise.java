package com.automatak.dnp3.impl;


public interface Promise<T> {
    void set(T value);
}
