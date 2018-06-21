#include "headers.h"

u8 IMAGE_INFO_SEND_FLAG=0;
///////////////////////////////////////////////////////////////////////////////
s16 readFile(char* fileName, u8 *buf, u16 l) {
	s16 len = -1;
	FILE *file;
	file = fopen(fileName, "rb");
	if (file != NULL) {
		len = fread(buf, 1, l, file);
		fclose(file);
		USART1_Printf("read file %s ok,len=%d.\r\n", fileName,len);
		return len;
	}
	USART1_Printf("read file %s failed,len=%d.\r\n", fileName,len);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
s16 writeFile(char* fileName, u8 *buf, u16 l) {
	s16 len = -1;
	FILE *file;
      
	file = fopen(fileName, "wb");
	if (file != NULL) {
		len = fwrite(buf, 1, l, file);
		fclose(file);
		USART1_Printf("write file %s ok,len=%d.\r\n", fileName,len);
            
		return len;
	}
	USART1_Printf("write file %s failed,len=%d.\r\n", fileName,len);
    fclose(file);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
s16 appendFile(char* fileName, u8 *buf, u16 l) {
	s16 len = -1;
	FILE *file;
	file = fopen(fileName, "ab");
	if (file != NULL) {
		len = fread(buf, 1, l, file);
		fclose(file);
		USART1_Printf("append file %s ok.\r\n", fileName);
		return len;
	}
	USART1_Printf("append file %s failed.\r\n", fileName);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
s16 truncateFile(char* fileName) {
	s16 len = -1;
	FILE *file;
	file = fopen(fileName, "w");
	if (file != NULL) {
		fclose(file);
		len = 0;
		USART1_Printf("truncate file %s ok.\r\n", fileName);
		return len;
	}
	USART1_Printf("truncate file %s failed.\r\n", fileName);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
extern DevInfo dev;
extern image  image1;
extern ModuleSN moduleSN;
s16 saveDevInfo(void) {
	return writeFile(DEV_FILE_NAME, (u8*)&dev, sizeof(DevInfo));
}
s16 readDevInfo(void) {
	return readFile(DEV_FILE_NAME, (u8*)&dev, sizeof(DevInfo));
}
//s16 saveimageInfo(void) {
//   if(IMAGE_INFO_SEND_FLAG==1)
//     {
//       IMAGE_INFO_SEND_FLAG=0;
//       fdelete("imageinfo");
//	 return writeFile("imageinfo", (u8*)&image1, sizeof(image));
//     }
//}
//s16 readimageInfo(void) {
//	return readFile("imageinfo", (u8*)&image1, sizeof(image));
//}
s16 saveModuleSN(void) {
	return writeFile(MODULESN_FILE_NAME, (u8*)&moduleSN, sizeof(ModuleSN));
}
s16 readModuleSN(void) {
	return readFile(MODULESN_FILE_NAME, (u8*)&moduleSN, sizeof(ModuleSN));
}

///////////////////////////////////////////////////////////////////////////////
u16 updateOfflineFile(OfflineInfo *info) {
	FILE *file;
	OfflineInfo data;
	u32 len=0;
	u16 cnt = 0;
	u8 not_found = 1;
	
	file = fopen(OFFLINE_FILE_NAME, "rb+");
	if (file == NULL) {
		//文件不存在
		file = fopen(OFFLINE_FILE_NAME, "wb+");
		if (file!=NULL) {
			fwrite(info, 1, sizeof(OfflineInfo), file);
			fclose(file);
			cnt = 1;
		}
	} else {
		fseek(file, 0, SEEK_SET);
		while (!feof(file)) {
			if (fread(&data, 1, sizeof(OfflineInfo), file)==sizeof(OfflineInfo)) {
				cnt ++;
				if ((data.id==info->id) && (data.Module_SN == info->Module_SN)) {
					not_found = 0;
					fseek(file, len, SEEK_SET);
					fwrite(info, 1, sizeof(OfflineInfo), file);
					fseek(file, 0, SEEK_END);
					cnt = ftell(file)/sizeof(OfflineInfo);
					break;
				} else {
					len += sizeof(OfflineInfo);
				}
			}
		}
		if (not_found == 1) {
			fwrite(info, 1, sizeof(OfflineInfo), file);
			cnt++;
		}
		fclose(file);
	}
    fclose(file);
	return cnt;
}
///////////////////////////////////////////////////////////////////////////////
u8 updateModuleInfo(ModuleInfo *info)
{
    u8 count=0;
    u8 len=0;
    u8 not_found = 1;
    FILE *file;
    ModuleInfo moduleinfo;
//    len=sizeof(moduleinfo);
	file = fopen(MODULE_FILE_NAME, "rb+");
	if (file == NULL) {
		//文件不存在
		file = fopen(MODULE_FILE_NAME, "wb+");
		if (file!=NULL) {
			fwrite(info, 1, sizeof(ModuleInfo), file);
			fclose(file);
			count = 1;
		}else {
	       USART1_Printf("open %s failed2.\r\n",MODULE_FILE_NAME);
	       return count;
	      } 
	} else {    
        fseek(file, 0, SEEK_SET);	    
        while (!feof(file)) {
            if (fread(&moduleinfo,1,sizeof(ModuleInfo), file) == sizeof(ModuleInfo)) {
                count++;
                if (moduleinfo.Module_SN == info->Module_SN){
                    not_found = 0;
					fseek(file, len, SEEK_SET);
					fwrite(info, 1, sizeof(ModuleInfo), file);									
					fseek(file, 0, SEEK_END);
					count = ftell(file)/sizeof(ModuleInfo);
					break;
				} else {
					len += sizeof(ModuleInfo);
				}
            }
        }
        if (not_found == 1) {
			fwrite(info, 1, sizeof(ModuleInfo), file);
			count++;
		}
	fclose(file);
  }
    fclose(file);
    return count;      
}
///////////////////////////////////////////////////////////////////////////////
u16 updateErrorInfoFile(ErrorInfo*info) {
	FILE *file;
	ErrorInfo data;
	u32 len=0;
	u16 cnt = 0;
	u8 not_found = 1;
	
	file = fopen(ERROR_FILE_NAME, "rb+");
	if (file == NULL) {
		//文件不存在
		file = fopen(ERROR_FILE_NAME, "wb+");
		if (file!=NULL) {
			fwrite(info, 1, sizeof(ErrorInfo), file);
			fclose(file);
			cnt = 1;
		}
	} else {
		fseek(file, 0, SEEK_SET);
		while (!feof(file)) {
			if (fread(&data, 1, sizeof(ErrorInfo), file)==sizeof(ErrorInfo)) {
				cnt ++;
				if (data.Module_SN == info->Module_SN&&data.Errortpye==info->Errortpye) {
					not_found = 0;
					fseek(file, len, SEEK_SET);
					fwrite(info, 1, sizeof(ErrorInfo), file);
					fseek(file, 0, SEEK_END);
					cnt = ftell(file)/sizeof(ErrorInfo);
					break;
				} else {
					len += sizeof(ErrorInfo);
				}
			}
		}
		if (not_found == 1) {
			fwrite(info, 1, sizeof(ErrorInfo), file);
			cnt++;
		}
		fclose(file);
	}
    fclose(file);
	return cnt;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
