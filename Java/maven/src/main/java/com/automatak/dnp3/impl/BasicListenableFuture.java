package com.automatak.dnp3.impl;


import com.automatak.dnp3.ListenableFuture;

import java.util.LinkedList;
import java.util.List;

class BasicListenableFuture<T> implements ListenableFuture<T>, Promise<T> {

    private final Object mutex = new Object();
    private final List<CompletionListener<T>> listeners = new LinkedList<CompletionListener<T>>();
    private T value = null;


    public T get() throws InterruptedException
    {
        synchronized (mutex)
        {
            while(value == null) mutex.wait();
            return value;
        }
    }

    public void addListener(CompletionListener<T> listener)
    {
       if(listener == null) throw new IllegalArgumentException("listener cannot be null");
       synchronized (mutex)
       {
           if(value == null) listeners.add(listener);
           else listener.onComplete(value);
       }
    }

    public void set(T value)
    {
        if(value == null) throw new IllegalArgumentException("value cannot be null");
        synchronized (mutex)
        {
            if(this.value == null) {
                this.value = value;
                mutex.notifyAll();
                for(CompletionListener<T> l: listeners) l.onComplete(value);
                listeners.clear();
            }
            else throw new IllegalStateException("value has already been set");
        }
    }

}
