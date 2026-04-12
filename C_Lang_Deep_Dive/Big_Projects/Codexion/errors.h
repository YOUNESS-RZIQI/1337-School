#include "codexion.h"

short null_error_message()
{
    write(2, "! ! ! Error (type: NULL return) ! ! !", 38);
    return (1);
}

short input_error_message()
{
	write(2, "! ! ! Error in Input ! ! !\n", 28);
    return (1);
}
