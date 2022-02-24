#ifndef FLY_RESOURCE_H
#define FLY_RESOURCE_H

#include <string>
namespace Fly {
    class Resource {
    public:
        const std::string& GetName() const { return m_Name; }

        virtual ~Resource() {}
    protected:
        std::string m_Name;
    };
}
#endif /* End of FLY_RESOURCE */
