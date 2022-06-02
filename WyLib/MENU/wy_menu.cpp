#include "common.h"
#include <stack>
namespace menu
{

    typedef struct
    {
        char *caption;
        void (*mid)(void);
        void (*left)(void);
        void (*right)(void);
    } MenuTypedef;

    void keyUpOption(void);
    void keyDownOption(void);
    void keyLeftOption(void);
    void keyMidOption(void);
    void keyRightOption(void);

    extern std::stack<MenuTypedef *> menuStack;

} // namespace menu

namespace menu
{
    std::stack<MenuTypedef *> menuStack;
    void keyUpOption(void)
    {
        
    }
} // namespace menu
