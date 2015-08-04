
#include "MasterConversions.h"

#include "TaskCallbackAdapter.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			std::vector<asiodnp3::Header> MasterConversions::ConvertToVectorOfHeaders(IEnumerable<Header^>^ headers)
			{
				std::vector<asiodnp3::Header> ret;
				for each(auto header in headers)
				{
					asiodnp3::Header output;
					if (Convert(header, output))
					{
						ret.push_back(output);
					}
				}
				return ret;
			}

			bool MasterConversions::Convert(Header^ header, asiodnp3::Header& output)
			{
				switch (header->qualifier)
				{
				case(QualifierCode::ALL_OBJECTS) :
				{
					output = asiodnp3::Header::AllObjects(header->group, header->variation);
					return true;
				}
				case(QualifierCode::UINT8_CNT) :
				{
					auto value = dynamic_cast<CountHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertCount8(value);
						return true;
					}
				}
				case(QualifierCode::UINT16_CNT) :
				{
					auto value = dynamic_cast<CountHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertCount16(value);
						return true;
					}
				}
				case(QualifierCode::UINT8_START_STOP) :
				{
					auto value = dynamic_cast<RangeHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertRange8(value);
						return true;
					}
				}
				case(QualifierCode::UINT16_START_STOP) :
				{
					auto value = dynamic_cast<RangeHeader^>(header);
					if (value == nullptr)
					{
						return false;
					}
					else
					{
						output = ConvertRange16(value);
						return true;
					}
				}
				default:
					return false;
				}
			}
		
			asiodnp3::Header MasterConversions::Convert(Header^ header)
			{
				return asiodnp3::Header::AllObjects(header->group, header->variation);
			}

			asiodnp3::Header MasterConversions::ConvertCount8(CountHeader^ header)
			{
				return asiodnp3::Header::Count8(header->group, header->variation, static_cast<uint8_t>(header->count));
			}

			asiodnp3::Header MasterConversions::ConvertCount16(CountHeader^ header)
			{
				return asiodnp3::Header::Count16(header->group, header->variation, header->count);
			}

			asiodnp3::Header MasterConversions::ConvertRange8(RangeHeader^ header)
			{
				return asiodnp3::Header::Range8(header->group, header->variation, static_cast<uint8_t>(header->start), static_cast<uint8_t>(header->stop));
			}

			asiodnp3::Header MasterConversions::ConvertRange16(RangeHeader^ header)
			{
				return asiodnp3::Header::Range16(header->group, header->variation, header->start, header->stop);
			}

			opendnp3::TaskConfig MasterConversions::Convert(TaskConfig^ config, ITaskCallback^ wrapper)
			{			
				return opendnp3::TaskConfig(Convert(config->taskId), CreateTaskCallback(wrapper), opendnp3::User(config->user->Number));
			}

			opendnp3::TaskConfig MasterConversions::Convert(TaskConfig^ config)
			{
				return opendnp3::TaskConfig(Convert(config->taskId), CreateTaskCallback(config->callback), opendnp3::User(config->user->Number));
			}

			opendnp3::TaskId MasterConversions::Convert(TaskId^ id)
			{
				return id->IsValid ? opendnp3::TaskId::Defined(id->Value) : opendnp3::TaskId::Undefined();
			}

			TaskId^ MasterConversions::Convert(const opendnp3::TaskId& id)
			{
				return id.IsDefined() ? TaskId::Defined(id.GetId()) : TaskId::Undefined;
			}

			TaskInfo^ MasterConversions::Convert(const opendnp3::TaskInfo& info)
			{
				return gcnew TaskInfo((MasterTaskType)info.type, (TaskCompletion)info.result, MasterConversions::Convert(info.id), Conversions::Convert(info.user));
			}

			opendnp3::ITaskCallback* MasterConversions::CreateTaskCallback(ITaskCallback^ callback)
			{
				return (callback == nullptr) ? nullptr : TaskCallbackAdapter::Create(callback);
			}
		}
	}
}