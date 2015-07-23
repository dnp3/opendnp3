using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Type that DNP3 authority provides to initiate a user status change request
    /// </summary>
    public class UserStatusChange
    {

        /// <summary>
        /// Constructor for symmetric updates that never have a user public key
        /// </summary>
        /// <param name="keyChangeMethod"></param>
        /// <param name="userOperation"></param>
        /// <param name="statusChangeSeqNum"></param>
        /// <param name="userRole"></param>
        /// <param name="userRoleExpDays"></param>
        /// <param name="userName"></param>
        /// <param name="certificationData"></param>
        public UserStatusChange(
            KeyChangeMethod keyChangeMethod,
            UserOperation userOperation,
            System.UInt32 statusChangeSeqNum,
            System.UInt16 userRole,
            System.UInt16 userRoleExpDays,
            String userName,            
            byte[] certificationData) : 
                this(keyChangeMethod, userOperation, statusChangeSeqNum, userRole, userRoleExpDays, userName, null, certificationData)
        {
            
        }       
                
        UserStatusChange(
            KeyChangeMethod keyChangeMethod,
            UserOperation userOperation,
	        System.UInt32 statusChangeSeqNum,
	        System.UInt16 userRole,
            System.UInt16 userRoleExpDays,
            String userName,
            byte[] userPublicKey,
            byte[] certificationData)
        {
            this.keyChangeMethod = keyChangeMethod;
            this.userOperation = userOperation;
            this.statusChangeSeqNum = statusChangeSeqNum;
            this.userRole = userRole;
            this.userRoleExpDays = userRoleExpDays;
            this.userName = userName;
            this.userPublicKey = userPublicKey;
            this.certificationData = certificationData;
        }       

        public readonly KeyChangeMethod keyChangeMethod;
        public readonly UserOperation userOperation;
        public readonly System.UInt32 statusChangeSeqNum;
        public readonly System.UInt16 userRole;
        public readonly System.UInt16 userRoleExpDays;
        public readonly String userName;
        public readonly byte[] userPublicKey;
        public readonly byte[] certificationData;
		
    }
}
