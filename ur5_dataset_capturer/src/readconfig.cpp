#include "readconfig.h"

//==================================================================
//function name：  ConfigFileRead
 //author： Ning Chuanfang
 //date： 2020-05-15
 //function：  
 //          read config file into map
 //input parameters：
 //          filename : config file name 
 //return：  map<string,string>  content of config file
 //          
 //modify：
 //
//==================================================================
map<string, string> ConfigFileRead(const std::string &filename) {
  map<string, string> m_mapConfigInfo;
  ifstream configFile;
  configFile.open(filename.c_str());
  string str_line;
  if (configFile.is_open()) {
    while (!configFile.eof()) {
      getline(configFile, str_line);
      if (str_line.compare(0, 1, "#") == 0) {
        continue;
      }
      size_t pos = str_line.find('=');
      if (pos != str_line.npos) {
        string str_key = str_line.substr(0, pos);
        string str_value = str_line.substr(pos + 1);
        m_mapConfigInfo.insert(pair<string, string>(str_key, str_value));
      }
    }
  } else {
    cout << "Cannot open config file:" << filename  <<endl;
    exit(-1);
  }
  return(m_mapConfigInfo);
}

//==================================================================
//function name：  mapfinds
 //author： Ning Chuanfang
 //date： 2020-05-15
 //function：  
 //          find config string in map
 //input parameters：
 //          m_mapConfigInfo content map of config file
 //          keystr config parameter name
 //return：  string  config parameter value
 //          
 //modify：
 //
//==================================================================
std::string mapfinds(map<string, string> m_mapConfigInfo,string keystr){
  map<string, string>::iterator iter_configMap;
  string valuestr;
  iter_configMap = m_mapConfigInfo.find(keystr.c_str());
  if(iter_configMap != m_mapConfigInfo.end())
    valuestr = iter_configMap->second;
  return(valuestr);
}
//==================================================================
//function name：  mapfindf
 //author： Ning Chuanfang
 //date： 2020-05-15
 //function：  
 //          find config string in map
 //input parameters：
 //          m_mapConfigInfo content map of config file
 //          str config parameter name
 //return：  float  config parameter value
 //          
 //modify：
 //
//==================================================================
void mapfindf(map<string, string> m_mapConfigInfo,string keystr,float &valuef){
  map<string, string>::iterator iter_configMap;
  string valuestr;
  iter_configMap = m_mapConfigInfo.find(keystr.c_str());
  if(iter_configMap != m_mapConfigInfo.end()){
    valuestr = iter_configMap->second;
    valuef = atof(valuestr.c_str());
  }
}
//==================================================================
//function name：  readconfig
 //author： Ning Chuanfang
 //date： 2020-05-15
 //function：  
 //          read config file into map 
 //          get initial parameter from map
 //          
 //modify：
 //
//==================================================================
void readconfig(){
  map<string, string> m_mapConfigInfo = ConfigFileRead("./conf/setting.conf");
  STLFileName = mapfinds(m_mapConfigInfo,"STLFileName");
  pVSFileName = mapfinds(m_mapConfigInfo,"pVSFileName");
  pFSFileName = mapfinds(m_mapConfigInfo,"pFSFileName");
  rgbTopic = mapfinds(m_mapConfigInfo,"rgbTopic");
  depthTopic = mapfinds(m_mapConfigInfo,"depthTopic");
  jsonFilePath = mapfinds(m_mapConfigInfo,"jsonFilePath");
  mapfindf(m_mapConfigInfo,"zNear",zNear);
  mapfindf(m_mapConfigInfo,"zFar",zFar);
  mapfindf(m_mapConfigInfo,"fx",fx);
  mapfindf(m_mapConfigInfo,"fy",fy);
  mapfindf(m_mapConfigInfo,"cx",cx);
  mapfindf(m_mapConfigInfo,"cy",cy);
  mapfindf(m_mapConfigInfo,"caml",caml);
  mapfindf(m_mapConfigInfo,"LEGO_POSITION_X",LEGO_POSITION_X);
  mapfindf(m_mapConfigInfo,"LEGO_POSITION_Y",LEGO_POSITION_Y);
  mapfindf(m_mapConfigInfo,"LEGO_POSITION_Z",LEGO_POSITION_Z);
  mapfindf(m_mapConfigInfo,"LEGO_ROTATION_EULER_Y",LEGO_ROTATION_EULER_Y);
  mapfindf(m_mapConfigInfo,"LEGO_ROTATION_EULER_P",LEGO_ROTATION_EULER_P);
  mapfindf(m_mapConfigInfo,"LEGO_ROTATION_EULER_R",LEGO_ROTATION_EULER_R);

  c1 = cos(DegToRad(LEGO_ROTATION_EULER_Y));
  s1 = sin(DegToRad(LEGO_ROTATION_EULER_Y));
  c2 = cos(DegToRad(LEGO_ROTATION_EULER_P));
  s2 = sin(DegToRad(LEGO_ROTATION_EULER_P));
  c3 = cos(DegToRad(LEGO_ROTATION_EULER_R));
  s3 = sin(DegToRad(LEGO_ROTATION_EULER_R));
  c1_h = cos(DegToRad(LEGO_ROTATION_EULER_Y)/2);
  s1_h = sin(DegToRad(LEGO_ROTATION_EULER_Y)/2);
  c2_h = cos(DegToRad(LEGO_ROTATION_EULER_P)/2);
  s2_h = sin(DegToRad(LEGO_ROTATION_EULER_P)/2);
  c3_h = cos(DegToRad(LEGO_ROTATION_EULER_R)/2);
  s3_h = sin(DegToRad(LEGO_ROTATION_EULER_R)/2);
}