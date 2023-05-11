struct term_struct {
	int coefficient;
	char symbol;
	int degree;
};


struct vars_struct {
	struct term_struct* polinom;
	char nameVar[256];
};