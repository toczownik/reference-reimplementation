#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "genicam_wrapper.h"

#define DEFAULT_DEVICE_INDEX 0
#define DEFAULT_STREAM_INDEX 0
#define DEFAULT_CAMERA_XML_INDEX 0
#define DEFAULT_FRAME_GRABBER_XML_INDEX 0
#define DEFAULT_FRAMES_TO_GRAB 100


#ifdef CALEO_SUPPORT
#include "caleo_meta_info.h"
#endif

#ifdef HERCULES_SUPPORT
#include "scd_meta_info.h"
#endif


#define DEFAULT_DEVICE_INDEX 0
#define DEFAULT_STREAM_INDEX 0
#define DEFAULT_CAMERA_XML_INDEX 0
#define DEFAULT_FRAME_GRABBER_XML_INDEX 0
#define DEFAULT_FRAMES_TO_GRAB 100

#define CUSTOM_HEADER 0
#define CALEO_HEADER 1
#define HERCULES_HEADER 2

#define MFG_BUFFER_INFO_META_INFO 0X7FFF1
#define MFG_BUFFER_INFO_HEADER_TYPE 0X7FFF2

#define MFG_BUFFER_INFO_TIFF_HEADER_SIZE 0x7FFF7
#define MFG_BUFFER_INFO_TIFF_HEADER_POINTER 0x7FFF8


#define UPDATE_TIMEOUT 100

#define OPT_NO_IMAGE	    (1 << 3)
#define OPT_SET_CONFIG	    (1 << 4)

/*
void version(){
    printf("\tGIT_SHORT_COMMIT_SHA:\t\t%s\n",GIT_SHORT_COMMIT_SHA);
    printf("\tGIT_COMMIT_SHA:\t\t\t%s\n",GIT_COMMIT_SHA);
    printf("\tGIT_CLEAN:\t\t\t%s\n",GIT_CLEAN);
    printf("\tGIT_TAG:\t\t\t%s\n",GIT_TAG);
    printf("\tGIT_DATE:\t\t\t%s\n",GIT_DATE);
    printf("\tVER_MAJOR:\t\t\t%s\n",VER_MAJOR);
    printf("\tVER_MINOR:\t\t\t%s\n",VER_MINOR);
    printf("\tVER_PATCH:\t\t\t%s\n",VER_PATCH);
    printf("\tVER_TWEAK:\t\t\t%s\n",VER_TWEAK);
    printf("\n");
    exit (-1);
}*/

void usage()
{
    printf("Frame grabber and Camera control application\n");
    printf("\n");
    printf("Options:\n");
    printf("\t-t\tselect device template [-t devicetemplate]\n");
    printf("\t-g\tselect mGenTL  [-g mGenTL]\n");
    printf("\t-i\tselect interface  [-i interface]\n");
    printf("\t-v\tdisplay message concerning project version and status\n");
    printf("\t-c\tspecify config file\n");
    printf("\t-h\tdisplay this message\n");
    printf("\n");
    exit(-1);
}

int set_node(const std::string& pNodeName, const std::shared_ptr<GenApi::CNodeMapRef>& pNodeMap, const std::string& pNodeValue){
    GenApi::CNodePtr node;
    try{
        node = pNodeMap->_GetNode(pNodeName.c_str());
    }
    catch (GENICAM_NAMESPACE::AccessException& e){
        std::cout << e.what();
        return -1;
    }
    GENAPI_NAMESPACE::EInterfaceType type;
    try{
        type = node->GetPrincipalInterfaceType();
    }
    catch(GENICAM_NAMESPACE::LogicalErrorException& e ){
        std::cout << e.what() <<std::endl;
        return -1;
    }
    try{
        switch(type){
            case GENAPI_NAMESPACE::intfIInteger: {
                auto intNode = (GENAPI_NAMESPACE::CIntegerPtr) node;
                intNode->SetValue(std::stoll(pNodeValue));
            }
                break;
            case GENAPI_NAMESPACE::intfIBoolean: {
                auto boolNode = (GENAPI_NAMESPACE::CBooleanPtr) node;
                boolNode->SetValue(std::stoi(pNodeValue));
            }
                break;
            case GENAPI_NAMESPACE::intfIEnumeration: {
                auto enumNode = (GENAPI_NAMESPACE::CEnumerationPtr) node;
                enumNode->FromString(pNodeValue.c_str());
            }
                break;
            case GENAPI_NAMESPACE::intfIFloat: {
                auto floatNode = (GENAPI_NAMESPACE::CFloatPtr) node;
                floatNode->SetValue(std::stof(pNodeValue));
            }
                break;
            case GENAPI_NAMESPACE::intfIString: {
                auto stringNode = (GENAPI_NAMESPACE::CStringPtr) node;
                stringNode->SetValue(pNodeValue.c_str());
            }
                break;
            case GENAPI_NAMESPACE::intfICommand: {
                auto commandNode = (GENAPI_NAMESPACE::CCommandPtr) node;
                commandNode->Execute();
            }
                break;
            default:
                std::cout << "Not supported node type" << std::endl;
                return -1;
        }
    }
    catch (GENICAM_NAMESPACE::AccessException& e){
        std::cout << e.what()<<std::endl;
        return -1;
    }
    catch (GENICAM_NAMESPACE::OutOfRangeException& e){
        std::cout << e.what() <<std::endl;
        return -1;
    }
    catch(GENICAM_NAMESPACE::InvalidArgumentException& e){
        std::cout << e.what() <<std::endl;
        return -1;
    }
    catch(GenTLException &e){
        std::cout << e.what() <<std::endl;
        return -1;
    }
    std::cout << "Node: " << pNodeName <<" set to value: " << pNodeValue << std::endl;
    return 0;
}

int main(int argc, char *argv[]) {
    int option;
    int selectedOptions = 0;
    std::string pathToConfigFile;
    std::string pathToStoreImagesAt;
    bool imageGeneratorEnabled = false;
    std::string genTLString;
    std::string interfaceString;
    GENICAM_NAMESPACE::gcstring deviceIDTemplateString = "";
    int frameGrabberXMLIndex = DEFAULT_FRAME_GRABBER_XML_INDEX;
    int deviceIndex = DEFAULT_DEVICE_INDEX;
    int cameraXMLIndex = DEFAULT_CAMERA_XML_INDEX;
    int streamIndex = DEFAULT_STREAM_INDEX;
    uint32_t framesToGrab = DEFAULT_FRAMES_TO_GRAB;
    uint templateInp = 0;
    uint gentlInp = 0;
    uint interfaceInp = 0;
    char *endPtr = nullptr;

    while ((option = getopt(argc, argv, "nc:hv")) != -1) {
        switch (option) {
            case 'h':
                usage();
                return 0;
            case 'c':
                selectedOptions |= OPT_SET_CONFIG;
                pathToConfigFile = optarg;
                break;
            case 'n':
                selectedOptions |= OPT_NO_IMAGE;
                break;
            case 'v':
                //version();
                break;
            default:
                break;
        }
    }
    if (selectedOptions == 0) {
        usage();
    }
    if(!(selectedOptions & OPT_SET_CONFIG)) {
        std::cout << "Path to config file has to be specified!\n";
        return 0;
    }
    std::unique_ptr<boost::property_tree::ptree> pt = std::make_unique<boost::property_tree::ptree>();
    try {
        boost::property_tree::ini_parser::read_ini(pathToConfigFile, *pt);
    } catch(...) {
        std::cout << "Configuration file (.INI) reading error" <<std::endl;
        return 0;
    }
    for (auto& section : *pt) {
        if (section.first != "general_settings") continue;
        for (auto& key : section.second){
            auto name = key.first;
            if(name == "GenTL"){
                genTLString = key.second.get_value<std::string>();
            }
            else if(name == "InterfaceName"){
                interfaceString = key.second.get_value<std::string>();
            }
            else if(name == "frameGrabberXMLIndex"){
                frameGrabberXMLIndex = key.second.get_value<int>();
            }
            else if(name == "deviceIndex"){
                deviceIndex = key.second.get_value<int>();
            }
            else if(name == "cameraXMLIndex"){
                cameraXMLIndex = key.second.get_value<int>();
            }
            else if(name == "streamIndex"){
                streamIndex = key.second.get_value<int>();
            }
            else if(name == "ImageGenerator"){
                imageGeneratorEnabled = key.second.get_value<bool>();
            }
            else if(name == "FramesToGrab"){
                framesToGrab = key.second.get_value<int>();
            }
            else if(name == "PathToStoreImagesAt"){
                pathToStoreImagesAt = key.second.get_value<std::string>();
            }
        }
    }
    System system(genTLString);
    auto interface = system.getInterface(interfaceString);
    auto frameGrabberNodeMap = interface.getFrameGrabberNodeMap(frameGrabberXMLIndex, imageGeneratorEnabled);
    for (auto& section : *pt) {
        if(section.first != "framegrabber_nodes") continue;
        for (auto& key : section.second) {
            // zmienić kolejność argumentów
            set_node(key.first, frameGrabberNodeMap, key.second.get_value<std::string>());
            std::cout << key.first << " " << key.second.get_value<std::string>() << std::endl;
        }
    }
    auto device = interface.getDevice(deviceIndex);
    auto stream = device.getStream(streamIndex);
    stream.getBuffers();
    stream.startAcquisition();
    stream.getFrame(pathToStoreImagesAt);
    stream.stopAcquisition();
    return 0;
}
