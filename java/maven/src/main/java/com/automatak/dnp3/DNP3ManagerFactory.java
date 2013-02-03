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

import com.automatak.dnp3.impl.ManagerImpl;

/**
 * Factory class used to retrieve the root DNP3 object.
 */
public class DNP3ManagerFactory {

    /**
     *
     * @param concurrency The number of threads that will be allocated to the underlying thread pool. This parameter is
     * @return Root management interface from which the entire class hierarchy is retrieved
     */
    public static DNP3Manager createManager(int concurrency)
    {
        return new ManagerImpl(concurrency);
    }

    /**
     * Retrieves a DNP3Manager with the thread pool automatically allocated to the number of processors/cores
     * @return Root management interface from which the entire class hierarchy is retrieved
     */
    public static DNP3Manager createDNP3ManagerWithDefaultConcurrency()
    {
        return new ManagerImpl(Runtime.getRuntime().availableProcessors());
    }

}
