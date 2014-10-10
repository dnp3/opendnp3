
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Master application code implements this interface to interface with stack
    /// </summary>
    public interface IMasterApplication
    {
        /// <summary>
        /// Get a count of milliseconds since Unix epoch for the purposes of time syncing
        /// </summary>
        /// <returns></returns>
        UInt64 GetMillisecondsSinceEpoch();
        
        /// <summary>
        /// Called whenever an IIN field is received
        /// </summary>
        /// <param name="iin"></param>
        void OnReceiveIIN(IINField iin);
        
        /// <summary>
        /// Task state notifications for built-in and user defined tasks        
        /// </summary>
        /// <param name="id"></param>
        /// <param name="state"></param>
        void OnTaskStateChange(TaskId id, TaskState state);
        
        /// <summary>
        /// Tells the master whether to assign class on startup
        /// </summary>
        /// <returns>if true, assign class is performed</returns>
        bool AssignClassDuringStartup();

        /// <summary>
        /// Returns a list of class assignments to perform during startup or restart
        /// Only called if AssignClassDuringStartup returns true
        /// </summary>
        /// <returns>An enumerable collection of class assignments</returns>
        IEnumerable<ClassAssignment> GetClassAssignments();
    }

    /// <summary>
    /// A default master application instance for demo purposes
    /// </summary>
    public class DefaultMasterApplication : IMasterApplication
    {
        private static IMasterApplication instance = new DefaultMasterApplication();

        private static DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, 0);

        public static IMasterApplication Instance
        {
            get
            {
                return instance;
            }
        }

        private DefaultMasterApplication() { }

        UInt64 IMasterApplication.GetMillisecondsSinceEpoch()
        {
            var ticks = DateTime.Now.ToUniversalTime().Subtract(epoch).Ticks;
            return (UInt64) (ticks / TimeSpan.TicksPerMillisecond);
        }

        void IMasterApplication.OnReceiveIIN(IINField iin)
        {
           // ignore these in the default application
        }

        void IMasterApplication.OnTaskStateChange(TaskId id, TaskState state)
        {
            // ignore these in the default application
        }

        bool IMasterApplication.AssignClassDuringStartup()
        {
            return true;
        }

        IEnumerable<ClassAssignment> IMasterApplication.GetClassAssignments()
        {
            var assignment1 = new ClassAssignment(3, 0, PointClass.Class2, Range.All());
            return new List<ClassAssignment>() { assignment1 };
        }
    }
}
