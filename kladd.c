struct Foo
{
	int m1;
	int m2;
};

int main(int argc, char **argv)
{
	struct Foo f = { 1, argc };
	
	return 0;
}
