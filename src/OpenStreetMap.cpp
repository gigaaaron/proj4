#include "OpenStreetMap.h"
#include <unordered_map>
#include <stdexcept>
#include <iostream>

struct COpenStreetMap::SImplementation
{

    struct SNode : public CStreetMap::SNode
    {
        TNodeID id;
        TLocation location;
        std::unordered_map<std::string, std::string> attributes;
        std::vector<std::string> attributesKey;
        SNode(TNodeID nodeid, double latitude, double longitude)
        {
            id = nodeid;
            location.first = latitude;
            location.second = longitude;
        }

        ~SNode() {}

        TNodeID ID() const noexcept { return id; }

        TLocation Location() const noexcept { return location; }

        std::size_t AttributeCount() const noexcept { return attributes.size(); }

        std::string GetAttributeKey(std::size_t index) const noexcept
        {
            try
            {
                return attributesKey.at(index);
            }
            catch (const std::out_of_range &excpt)
            {
                return "";
            }
        }

        bool HasAttribute(const std::string &key) const noexcept
        {
            return attributes.find(key) != attributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept
        {
            if (HasAttribute(key))
            {
                return attributes.find(key)->second;
            }
            else
            {
                return "";
            }
        }
    };

    struct SWay : public CStreetMap::SWay
    {
        TWayID id;
        std::vector<TNodeID> way;
        std::unordered_map<std::string, std::string> attributes;
        std::vector<std::string> attributesKey;

        SWay(TWayID wayid) { id = wayid; }

        ~SWay() {}

        TWayID ID() const noexcept { return id; }

        std::size_t NodeCount() const noexcept { return way.size(); }

        TNodeID GetNodeID(std::size_t index) const noexcept
        {
            {
                try
                {
                    return way.at(index);
                }
                catch (const std::out_of_range &excpt)
                {
                    return InvalidNodeID;
                }
            }
        }

        std::size_t AttributeCount() const noexcept { return attributes.size(); }

        std::string GetAttributeKey(std::size_t index) const noexcept
        {
            try
            {
                return attributesKey.at(index);
            }
            catch (const std::out_of_range &excpt)
            {
                return "";
            }
        }
        bool HasAttribute(const std::string &key) const noexcept
        {
            return attributes.find(key) != attributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept
        {
            if (HasAttribute(key))
            {
                return attributes.find(key)->second;
            }
            else
            {
                return "";
            }
        }
    };

    std::vector<std::shared_ptr<SNode>> nodes;
    std::vector<std::shared_ptr<SWay>> ways;

    SImplementation(std::shared_ptr<CXMLReader> src)
    {
        SXMLEntity entity;
        std::string obj;

        while (src->ReadEntity(entity))
        {
            if (entity.DType == SXMLEntity::EType::StartElement)
            {

                
                if (entity.DNameData == "node")
                {
                    TNodeID id;
                    double lat, lon;

                    try
                    {
                        id = std::stol(entity.DAttributes[0].second);
                        lat = std::stod(entity.DAttributes[1].second);
                        lon = std::stod(entity.DAttributes[2].second);
                    }

                    catch (std::invalid_argument)
                    {
                        continue;
                    }

                    auto node = std::make_shared<SNode>(id, lat, lon);
                    nodes.push_back(std::move(node));
                    obj = "node";
                }

                else if (!entity.DNameData.compare("tag"))
                {
                    std::string key = entity.DAttributes[0].second;
                    std::string value = entity.DAttributes[1].second;

                    if (obj == "node")
                    {
                        nodes[nodes.size() - 1]->attributes.insert({key, value});
                        nodes[nodes.size() - 1]->attributesKey.push_back(key);
                    }
                    else if (obj == "way")
                    {
                        ways[ways.size() - 1]->attributes.insert({key, value});
                        ways[ways.size() - 1]->attributesKey.push_back(key);
                    }
                    else
                    {
                       continue;
                    }
                }

                else if (!entity.DNameData.compare("way"))
                {
                    long id;
                    id = std::stol(entity.DAttributes[0].second);

                    auto way = std::make_shared<SWay>(id);
                    ways.push_back(std::move(way));
                    obj = "way";
                }

                else if (!entity.DNameData.compare("nd"))
                {
                    long ref;
                    ref = std::stol(entity.DAttributes[0].second);

                    ways[ways.size() - 1]->way.push_back(ref);
                }
            }
        }
    }

    ~SImplementation() {};

    std::size_t NodeCount() const noexcept { return nodes.size(); }

    std::size_t WayCount() const noexcept { return ways.size(); }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept
    {
        try
        {
            return nodes.at(index);
        }
        catch (std::out_of_range &excpt)
        {
            return nullptr;
        }
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept
    {
        for (int i = 0; i < NodeCount(); i++)
        {
            if (id == nodes[i]->id)
                return nodes[i];
        }
        
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept
    {
        try
        {
            return ways.at(index);
        }
        catch (std::out_of_range &excpt)
        {
            return nullptr;
        }
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept
    {
        for (int i = 0; i < WayCount(); i++)
        {
            if (id == ways[i]->id)
                return ways[i];
        }

        return nullptr;
    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) {
	DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){}

std::size_t COpenStreetMap::NodeCount() const noexcept {
	return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
	return DImplementation->WayCount();
}

std::shared_ptr<COpenStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
	return DImplementation->NodeByIndex(index);
}

std::shared_ptr<COpenStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
	return DImplementation->NodeByID(id);
}

std::shared_ptr<COpenStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
	return DImplementation->WayByIndex(index);
}

std::shared_ptr<COpenStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
	return DImplementation->WayByID(id);
}
