#pragma once
#include <string>
#include <vector>

class person
{
public:
	std::string first_name{};
	std::string last_name{};
	bool seen{ false };
	enum class MemberType
	{
		NA,
		MEMBER,
		LEADER,
		VISITOR,
	};
	MemberType member_type{ MemberType::NA };

	enum class AttendanceType
	{
		NA,
		NOT_TAKEN,
		PRESENT,
		NOT_PRESENT,
		VISITING,
	};
	struct Attendance
	{
		std::string date{};
		AttendanceType type{ AttendanceType::NA };
		int weeks_absent{ 99 };
	};
	std::vector<Attendance> attendance_list;

};

