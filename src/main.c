#define UNUSED_ARGUMENT(x) (void) x

int main(void *mboot, unsigned int magic_number)
{
    UNUSED_ARGUMENT(mboot);
    UNUSED_ARGUMENT(magic_number);
    return 0xDEADBEEF;
}