#ifndef INCLUDE_HPHP_EXT_EVENT_H_
#define INCLUDE_HPHP_EXT_EVENT_H_
#include "hphp/runtime/base/base-includes.h"

namespace HPHP
{
    class eventExtension: public Extension
    {
        public:
            eventExtension(): Extension("event"){}
            virtual void moduleInit()
            {
                _initEventBaseClass();
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
    };
}
#endif