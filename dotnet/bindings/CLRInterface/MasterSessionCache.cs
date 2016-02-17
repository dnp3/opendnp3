using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public class MasterSessionCache
    {
        IDictionary<UInt64, IMasterSession> sessions = new Dictionary<UInt64, IMasterSession>();

        public bool Remove(UInt64 sessionid)
        {
            lock (sessions)
            {
                return sessions.Remove(sessionid);
            }
        }

        public IMasterSession Add(UInt64 sessionid, IMasterSession session)
        {
            lock (sessions)
            {
                sessions[sessionid] = session;
                return new MasterSessionProxy(this, sessionid);
            }
        }

        // Can throw KeyNotFoundException
        public T Perform<T>(UInt64 sessionid, Func<IMasterSession, T> action)
        {
            lock (sessions)
            {                
                return action(sessions[sessionid]);
            }
        }

        // Can throw KeyNotFoundException
        public void Perform(UInt64 sessionid, Action<IMasterSession> action)
        {
            lock (sessions)
            {
                action(sessions[sessionid]);
            }
        }
    }
}
