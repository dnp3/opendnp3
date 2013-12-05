#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_

namespace opendnp3
{
	template<class T>
	class ErrorCode
	{
		public:

			static ErrorCode Success(T aValue)
			{
				return ErrorCode(aValue, 0);
			}

			static ErrorCode Failure(int aCode)
			{
				assert(aCode != 0);
				return ErrorCode(aCode);
			}

			bool IsError()
			{
				return code != 0;
			}
			
			int Code()
			{
				assert(code != 0);
				return code;
			}

			T Result()
			{
				assert(code == 0);
				return value;
			}

		private:
			ErrorCode(T aValue, int aCode): value(aValue), code(aCode)
			{}

			ErrorCode(int aCode): value(), code(aCode)
			{}

			T value;
			int code;
	};


}

#endif
