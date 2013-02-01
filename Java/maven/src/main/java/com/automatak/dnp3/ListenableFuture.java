/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
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
