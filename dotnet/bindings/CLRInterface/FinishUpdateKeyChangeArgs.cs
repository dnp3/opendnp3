using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public class FinishUpdateKeyChangeArgs
    {
        public FinishUpdateKeyChangeArgs(
            string username,
            string outstationName,
            User user,
            System.UInt32 keyChangeSequenceNum,
            byte[] masterChallengeData,
            byte[] outstationChallengeData,
            byte[] encryptedKeyData,
            UpdateKey updateKey)
        {
            this.username = username;
            this.outstationName = outstationName;
            this.user = user;
            this.keyChangeSequenceNum = keyChangeSequenceNum;
            this.masterChallengeData = masterChallengeData;
            this.outstationChallengeData = outstationChallengeData;
            this.encryptedKeyData = encryptedKeyData;
            this.updateKey = updateKey;
        }
				
	    /// The UTF-8 username shared by the authority and outstation
	    public readonly string username;

	    /// Organizationally unique name of the outstation
        public readonly string outstationName;

	    /// The user # assigned by the outstation associated w/ the UTF-8 encoded name
        public readonly User user;	

	    /// The KSQ specified by the outstation
        public readonly System.UInt32 keyChangeSequenceNum;

	    /// The challenge data that the master chose when it initiated the request
        public readonly byte[] masterChallengeData;

	    /// The challenge data that the outstation provided in its response
        public readonly byte[] outstationChallengeData;

	    /// The encrypted key data provided by the authority using the authority symmetric key
        public readonly byte[] encryptedKeyData;	

	    /// the plaintext update key that was derived by the master and signed by the authority
        public readonly UpdateKey updateKey;
    };
}
