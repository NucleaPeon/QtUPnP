#include <sys/utsname.h>
#include <QString>

namespace QSysLinux {

	const QString kernelVersion()
	{
	//	int maj = LINUX_VERSION_CODE >> 16;
	//	int min = ( LINUX_VERSION_CODE - ( maj << 16 ) ) >> 8;
	//	int pat = LINUX_VERSION_CODE - ( maj << 16 ) - ( min << 8 );
		struct utsname unamedata;
		uname(&unamedata);
		return QString(unamedata.release);

	}

	const QString kernelType()
	{
		struct utsname unamedata;
		uname(&unamedata);
		return QString(unamedata.sysname);
	}

}
