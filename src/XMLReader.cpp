#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation {
    std::shared_ptr < CDataSource > source;
    XML_Parser parser;
    std::queue < SXMLEntity > queue;

    SImplementation(std::shared_ptr < CDataSource > src) {
        source = src;
        parser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(parser, StartHandler);
        XML_SetEndElementHandler(parser, EndHandler);
        XML_SetCharacterDataHandler(parser, CharHandler);
        XML_SetUserData(parser, this);
    }

    static void StartHandler(void *userData, const XML_Char *name, const char **attributes) {
        SXMLEntity temp;
        temp.DNameData = name;
        temp.DType = SXMLEntity::EType::StartElement;
        std::vector< std::string > attribs;
        auto attPtr = attributes;

        while (*attPtr) {
            attribs.push_back(*attPtr);
            attPtr++;
        }

        for (int i = 0; i < attribs.size(); i += 2)
            temp.SetAttribute(attribs[i], attribs[i + 1]);

        // Access SImplementation instance through "this" ptr from userData
        SImplementation *obj = static_cast< SImplementation* > (userData);
        obj->queue.push(temp);
    }

    static void EndHandler(void *userData, const XML_Char *name) {
        SXMLEntity temp;
        temp.DNameData = name;
        temp.DType = SXMLEntity::EType::EndElement;

        // Access SImplementation instance through "this" ptr from userData
        SImplementation *obj = static_cast< SImplementation* > (userData);
        obj->queue.push(temp);
    }

    static void CharHandler(void *userData, const XML_Char *s, int len) {
        SXMLEntity temp;
        temp.DNameData = std::string(s, len);
        temp.DType = SXMLEntity::EType::CharData;

        // Access SImplementation instance through "this" ptr from userData
        SImplementation *obj = static_cast< SImplementation* > (userData);
        obj->queue.push(temp);
    }

    bool End() const {
        return source->End();
    }

    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false) {
        std::vector< char > buffer;

        // Read from source until element is processed and added to queue
        while (queue.empty()) {
            if (End())
                return false;
            if (source->Read(buffer, 669))
                XML_Parse(parser, buffer.data(), buffer.size(), buffer.size() < 669);
            else
                XML_Parse(parser, buffer.data(), 0, true);
        }
        if (queue.empty())
            return false;

        entity = queue.front();
        if (skipcdata) {
            while (entity.DType == SXMLEntity::EType::CharData) {
                queue.pop();
                entity = queue.front();
            }
        }
       
        queue.pop();
        return true;
    }
};

CXMLReader::CXMLReader(std::shared_ptr < CDataSource > source) {
    DImplementation = std::make_unique < SImplementation > (source);
}

CXMLReader::~CXMLReader() {}

bool CXMLReader::End() const {
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadEntity(entity, skipcdata);
}
