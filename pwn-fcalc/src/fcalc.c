// gcc -s -z execstack -o fcalc fcalc.c
// POC: 1 1 0
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#define SIZE 0x30

void add();
void sub();
void mul();
void divi();
void nop();

void (*zzz[0x10])(void);
double *fsp; // floating-point stack pointer
int fsc = -1; // floating-point stack counter

void add()
{
  fsp--;
  fsp[-1] += fsp[0];
  fsc--;
}

void sub()
{
  fsp--;
  fsp[-1] -= fsp[0];
  fsc--;
}

void mul()
{
  fsp--;
  fsp[-1] *= fsp[0];
  fsc--;
}

void divi()
{
  fsp--;
  fsp[-1] /= fsp[0];
  fsc--;
}

void nop()
{
  puts("nop");
}

void init()
{
  setvbuf(stdin, 0, 2, 0);
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stderr, 0, 2, 0);

  zzz[' ' - ' '] = nop;
  zzz['!' - ' '] = nop;
  zzz['"' - ' '] = nop;
  zzz['#' - ' '] = nop;
  zzz['$' - ' '] = nop;
  zzz['%' - ' '] = nop;
  zzz['&' - ' '] = nop;
  zzz['\'' - ' '] = nop;
  zzz['(' - ' '] = nop;
  zzz[')' - ' '] = nop;
  zzz['*' - ' '] = mul;
  zzz['+' - ' '] = add;
  zzz[',' - ' '] = nop;
  zzz['-' - ' '] = sub;
  zzz['.' - ' '] = nop;
  zzz['/' - ' '] = divi;
}

void hello()
{
  puts("======================================");
  puts("     ____  ___   __   __     ___ ");
  puts("    (  __)/ __) / _\\ (  )   / __)");
  puts("     ) _)( (__ /    \\/ (_/\\( (__ ");
  puts("    (__)  \\___)\\_/\\_/\\____/ \\___)");
  puts("    postfix calculator");
  puts("======================================");
  puts("Welcome to fcalc!");
  puts("");
  puts("Input your postfix expression, and I will calculate it for you.");
  puts("You can use +, -, *, /.");
  puts("");
  puts("In order to keep the presicion and the scale in control, the input");
  puts("number's abs value must be more than 1 and less than 100.0.");
  puts("Otherwise, some weird thing may happen. For example:");
  puts("   3.14f +  10000000000f - 10000000000f");
  puts("!= 3.14f + (10000000000f - 10000000000f)");
  puts("");
  puts("Enter your expression:");
}

int main()
{
  double *fstack = alloca(SIZE * sizeof(double));
  char *buf = alloca(SIZE);

  memset(fstack, 0, SIZE * sizeof(double));
  memset(buf, 0, SIZE);

  fsp = fstack;
  fsc = 0;

  init();
  hello();

  while (1)
  {
    int exp_len = read(0, buf, SIZE * sizeof(double));  // VUL: overflow to fstack (shellcode injection)
    if (exp_len >= SIZE)
      exp_len = SIZE;
    for (int i = 0; i < exp_len; i++)
    {
      if (buf[i] == '\n')
        break;

      if (' ' < buf[i] && buf[i] <= '0')  // VUL: buf[i] == '0' will call fsp (shellcode execution)
      {
        if (fsc >= 2)
        {
          // check if the num is valid
          double *p = fstack;
          for (int i = 0; i < SIZE; i++)
          {
            double num = fabs(*p);
            if (num != 0. && (num < 1. || num > 100.)) // CONSTRAIN: every byte of shellcode should pass this check
            {
              // puts("ERROR");
              printf("ERROR: %lf\n", num);
              exit(1);
            }
            p++;
          }
          zzz[buf[i] - ' ']();
        }
        else
        {
          puts("ERROR");
          exit(1);
        }
      }
      else if ('0' <= buf[i] && buf[i] <= '9')
      {
        if (fsc >= SIZE)
        {
          puts("ERROR");
          exit(1);
        }
        fsc++;
        *fsp = atof(&buf[i]);
        fsp++;
        while (buf[i + 1] == '.' || ('0' <= buf[i + 1] && buf[i + 1] <= '9'))
          i++;
      }
    }
    // result
    if (fsp > fstack) {
      printf("Result: %lf\n", fsp[-1]);
    }
  }
}
