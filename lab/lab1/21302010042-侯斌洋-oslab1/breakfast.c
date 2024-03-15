#define NOTHING 0
#define EGGCAKE 1
#define PANCAKE 2
#define MAX 1000

int window = NOTHING; // 0->nothing, 1->eggcake, 2->pancake
int eggcake_first = 0;
int pancake_first = 0;
int eggcake_num = 0;
int pancake_num = 0;

void put_eggcake()
{
    if (window == NOTHING && eggcake_num != 0)
    {
        window = EGGCAKE;
    }
}

void put_pancake()
{
    if (window == NOTHING && pancake_num != 0)
    {
        window = PANCAKE;
    }
}

void get_eggcake()
{
    if (window != EGGCAKE && eggcake_num > 0)
    {
        eggcake_first = (eggcake_first + 1) % MAX;
        eggcake_num--;
    }
}

void get_pancake()
{
    if (window != PANCAKE && pancake_num > 0)
    {
        pancake_first = (eggcake_first + 1) % MAX;
        pancake_num--;
    }
}

void enqueue_eggcake()
{
    if (eggcake_num <= MAX)
    {
        eggcake_num++;
    }
}

void enqueue_pancake()
{
    if (pancake_num <= MAX)
    {
        pancake_num++;
    }
}