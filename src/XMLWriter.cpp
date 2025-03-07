#include "XMLWriter.h"
#include "StringUtils.h"

struct CXMLWriter::SImplementation {
    std::shared_ptr < CDataSink > stnk;
    std::vector < SXMLEntity > written;
    SXMLEntity::EType last;

    SImplementation(std::shared_ptr < CDataSink > sink)
    {
        stnk = sink;
    }

    bool Flush() {
        for (int i = 0; i < written.size(); i++)
            WriteEntity({SXMLEntity::EType::EndElement, written.back().DNameData});
        
        return true;
    }

    bool WriteEntity(const SXMLEntity &entity) {
        std::string ent = ReplaceChars(entity.DNameData);
        
        // Start element
        if (entity.DType == SXMLEntity::EType::StartElement) {
            if (last == SXMLEntity::EType::CharData)
                return false;
            
            if (written.empty())
                ent = "<" + ent;
            else
                ent = "\n" + std::string(written.size(), '\t') + "<" + ent;
            written.push_back(entity);

            // Attributes
            for (int i = 0; i < entity.DAttributes.size(); i++)
                ent += " " + ReplaceChars(entity.DAttributes[i].first) + "=" + "\"" + ReplaceChars(entity.DAttributes[i].second) + "\"";

            ent += ">";
            last = SXMLEntity::EType::StartElement;

        // End Element
        } else if (entity.DType == SXMLEntity::EType::EndElement) {
            // Top element on stack has to match
            if (written.empty() || written.back().DNameData != ent)
                return false;
            
            written.pop_back();

            if (last == SXMLEntity::EType::CharData || last == SXMLEntity::EType::StartElement)
                ent = "</" + ent + ">";
            else
                ent = "\n" + std::string(written.size(), '\t') + "</" + ent + ">";
            last = SXMLEntity::EType::EndElement;

        // Char Data
        } else if (entity.DType == SXMLEntity::EType::CharData) {
            
            if (written.empty())
                return false;
            last = SXMLEntity::EType::CharData;
        
        // Complete Element
        }  else {

            if (last == SXMLEntity::EType::CharData)
                return false;
            
            if (written.empty())
                ent = "<" + ent;
            else
                ent = "\n" + std::string(written.size(), '\t') + "<" + ent;
            
            // Attributes
            for (int i = 0; i < entity.DAttributes.size(); i++)
                ent += " " + ReplaceChars(entity.DAttributes[i].first) + "=" + "\"" + ReplaceChars(entity.DAttributes[i].second) + "\"";
            
            ent += "/>";
            last = SXMLEntity::EType::CompleteElement;
        }
        
        // Write ent to sink
        std::vector<char> entchar;
        for (int i = 0; i < ent.length(); i++)
            entchar.push_back(ent[i]);
        
        stnk->Write(entchar);
        return true;

    }

    std::string ReplaceChars(std::string str) {
        str = StringUtils::Replace(str, "&", "&amp;");
        str = StringUtils::Replace(str, "\"", "&quot;");
        str = StringUtils::Replace(str, "'", "&apos;");
        str = StringUtils::Replace(str, "<", "&lt;");
        str = StringUtils::Replace(str, ">", "&gt;");

        return str;
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr< CDataSink > sink) {
	DImplementation = std::make_unique < SImplementation >(sink);
}

CXMLWriter::~CXMLWriter() {}

bool CXMLWriter::Flush() {
	return DImplementation->Flush();
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
	return DImplementation->WriteEntity(entity);
}
