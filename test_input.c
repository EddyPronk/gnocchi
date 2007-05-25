void action()
{
}

void func_empty()
{
}

void func_if_with_assignment(int c)
{
  int a = 0;
  if(c)
	{
	  a = 1;
	}
}

void func_if_with_function_call(int c)
{
  int a = 0;
  if(c)
	{
	  action();
	}
}

void func_if_else(int c)
{
  int a = 0;
  if(c)
	{
	  a = 1;
	}
  else
	{
	  a = 2;
	}
}
void func_if_else_2(int c)
{
  int a = 0;
  if(c)
	{
	  if(c)
		{
		  a = 1;
		}
	  else
		{
		  a = 1;
		}
	}
  else
	{
	  if(c)
		{
		  a = 1;
		}
	  else
		{
		  a = 1;
		}
	}
}

void example_1(char c)
{
  int ca = 0;
  int cb = 0;
  int cc = 0;
  int cC = 0;
  int cOther = 0;

  if ( c == 'a' )
	ca++;
  if ( c == 'b' )
	cb++;
  if ( c == 'c' )
	cc++;
  if ( c == 'C' )
	cC++;

  if ( c != 'a' && c != 'b' && c != 'c' && c != 'C')
  	cOther++;
  
}

void example_2(char ch)
{
  int actr = 0;
  int bctr = 0;
  int cctr = 0;
  int dctr = 0;

  if ( ch == 'a' )
	{
	  actr++;
	  Func_a();
	}
  if ( ch == 'b' )
	{
	  bctr++;
	  Func_b();
	}
  if ( ch == 'c' )
	{
	  cctr++;
	  Func_c();
	}
  if ( ch == 'd' )
	{
	  dctr++;
	  Func_d();
	}
}

void example_3(char c)
{
  int ca = 0;
  int cb = 0;
  int cc = 0;
  int cC = 0;
  int cOther = 0;

  switch ( c )
	{
	case 'a':
	  ca++;
	  break;
	case 'b':
	  ca++;
	  break;
	case 'c':
	  ca++;
	  break;
	case 'C':
	  ca++;
	  break;
	default:
	  cOther++;
	}
}

void example_4(int A, int B)
{
  int S0 = 0;
  int S1 = 0;

  if (A == B)
	{
	  S0 = S0;
	}
  if (A == B)
	{
	  S1 = S1;
	}
}
void example_5(int A, int B, int C)
{
  int S0 = 0;
  int S1 = 0;

  if (A == B)
	{
	  S0 = S0;
	}
  if (A == C)
	{
	  S1 = S1;
	}
}
