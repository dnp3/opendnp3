package com.automatak.dnp3;

public interface ListenableFuture<T> {

    public interface CompletionListener<T>
    {
        void onComplete(T value);
    }

    /**
     * Blocks until the asynchronous operation completes
     * @return
     */
    T get() throws InterruptedException;

    /**
     * Adds a callback that is invoked when the asynchronous operation completes.
     * If the operation is already complete, the callback is invoked from the calling thread.
     * @param listener
     */
    void addListener(CompletionListener<T> listener);

}
