/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3.mock;

import com.automatak.dnp3.LogEntry;
import com.automatak.dnp3.LogSubscriber;

import java.util.Date;

/**
 * Singleton that prints log information as it is received
 */
public class PrintingLogSubscriber implements LogSubscriber {
    private static PrintingLogSubscriber ourInstance = new PrintingLogSubscriber();

    @Override
    public void onLogEntry(LogEntry le)
    {
        Date now = new Date();
        System.out.println(now + " - " + le.getLogLevel().name() + " - " + le.getLoggerName() + " - " + le.getMessage());

    }

    public static PrintingLogSubscriber getInstance() {
        return ourInstance;
    }

    private PrintingLogSubscriber() {
    }
}
