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
package com.automatak.dnp3.impl;


import com.automatak.dnp3.*;

class MasterImpl extends StackBase implements Master {

    private long nativePointer;
    private CommandProcessor processor;

    public MasterImpl(long nativePointer)
    {
        super(nativePointer);
        this.nativePointer = nativePointer;
        this.processor = new CommandProcessorImpl(get_native_command_processor(nativePointer));
    }

    @Override
    public CommandProcessor getCommandProcessor()
    {
        return this.processor;
    }

    @Override
    public void shutdown()
    {
        shutdown_native(nativePointer);
    }

    private native long get_native_command_processor(long nativePointer);
    private native void shutdown_native(long nativePointer);
}
