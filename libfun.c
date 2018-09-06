#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
//word_count的宏定义
#define OUT     	0   //outside word
#define IN      	1
//get_line
#define MAXLINE		1000


//统计屏幕输入的行数, 单词数, 以及字符数
void word_count();
//读取屏幕上一行的字符串到line中
int get_line(char* line, unsigned int lim);
//计算str的长度
int str_len(const char* str);
//将t拼接到s后面
char* str_cat(char* s, const char* t);
//将t的前n个字符拼接到s后面
char* str_cat_n(char* s, const char* t, unsigned int n);
//得到数字x中从pos开始后n位的数值
int get_bits(unsigned int x, int pos, unsigned int n);
//字符串转整型
int atoi_v1(const char* str);
//字符串转整型, 可以忽略空格,区分正负号
int atoi_v2(const char* str);
//整型转字符串
int itoa(long long int n, char* str);
//浮点型转字符串
int ftoa(double n, char* str, unsigned int precision);
//反转字符串
char* reverse(char* str);
//将n转换为以b为低的数,b表示进制
void itob(long long int n, char* str, unsigned int b);
//字符串拷贝
char* str_cpy(char* dest, const char* source);
//字符串拷贝,拷贝n个
char* str_cpy_n(char* dest, const char* source, unsigned int n);
//字符串比较
int str_cmp(char* s, char* t);
//字符串比较前n位
int str_cmp_n(char* s, char* t, unsigned int n);
//设置ptr所指向内存的前num个byte为value
void* mem_set(void* ptr, int value, unsigned int num);
//将source的前num的byte拷贝到dest中, mem_cpy等价于memmove
void* mem_cpy(void* dest, const void* source, unsigned int num );
//从stream中读取num-1个字符串到str中
char* fget_s (char* str, int num, FILE* stream);
//将str写入到stream中
int fput_s (const char* str, FILE* stream);

//打印
int print_f(const char* fmt, ...);

//方便打印字符串
void print_str(const char* str){
	int len = str_len(str);
	int i;
	for(i = 0; i < len; i++){
		printf("%c", str[i]);
	}
	printf("\n");

}

//测试数据
struct {
  char name[40];
  int age;
} person, person_copy;


int main(){
	// char dest[20];
	// char source[20] = "1234567";
	// char s[] = "89";
	
	// print_f("dest:%s\n", str_cpy(dest, source));
	// print_f("len of dest:%d\n", str_len(dest));
	// printf("dest+s,n:%s\n", str_cat_n(dest, s, 5));
	// print_f("dest+s:%s\n", str_cat(dest, s));
	// print_f("reverse:%s\n", reverse(dest));
	// print_f("copy to dest:%s\n", str_cpy(dest, source));
	// print_f("copy to dest, n:%s\n", str_cpy_n(dest, source, 4));
	// print_f("cmp: %d\n", str_cmp(dest, source));
	// print_f("cmp,n: %d\n", str_cmp_n(dest, source, 4));

	// print_f("%.6f\n", 15.666);
	// print_f("%d\n", 16);
	// print_f("%lld\n", 65000000000000000L);
	// print_f("%c\n", 'c');
	// print_f("%s\n", "12323");
	// print_f("%x\n", 16);	
	// print_f("%lo\n", 65000000000000000L);
	// print_f("%u\n", 12);
	// printf("%d%%\n", 15);

	// char str[] = "almost every programmer should know memset!";
	// mem_set(str, '0' ,6);
	// print_f("%s\n", str);

	// char myname[] = "Pierre de Fermat";
	// /* using memcpy to copy string: */
	// mem_cpy ( person.name, myname, strlen(myname)+1 );
	// person.age = 46;

	// /* using memcpy to copy structure: */
	// mem_cpy ( &person_copy, &person, sizeof(person) );
	// print_f ("person_copy: %s, %d \n", person_copy.name, person_copy.age );

	// //地址重合检测
	// char buf[100] = "1234567";
	// mem_cpy(buf+2, buf, 7);
	// print_f("%s\n", buf+2);
	// print_f("%s\n", buf);

	// char buf1[100] = "1234567";
	// str_cpy(buf1+2, buf1);
	// print_f("%s\n", buf1+2);
	// print_f("%s\n", buf1);

	// char buf2[100] = "1234567";
	// str_cpy_n(buf2+2, buf2, str_len(buf2));
	// print_f("%s\n", buf2+2);
	// print_f("%s\n", buf2);	

	// char str[20];
	// print_f("%s\n", fget_s(str, 10, stdin));
	// fput_s(str, stdout);
	

}


int fput_s(const char* str, FILE* stream){
	assert(str);
	int c;
	while(c = *str++) putc(c, stream);
	return ferror(stream) ? EOF : 0;
}


//注意第num用于放'\0'
char* fget_s (char* str, int num, FILE* stream){
	assert(str), assert(num > 0);
	register int c; 
	register char* cs;
	cs = str;

	while(--num > 0 && (c = getc(stream)) != EOF)
		if((*cs++ = c) == '\n') break;

	*cs = '\0';
	//考虑程序运行,无输入直接退出或者无输入字符情况
	return (c == EOF || cs == str) ? NULL : str; 
}


/*
	目前支持的格式:
		print_f("%.6f\n", 15.666);
		print_f("%d\n", 16);
		print_f("%lld\n", 65000000000000000L);
		print_f("%c\n", 'c');
		print_f("%s\n", "12323");
		print_f("%x\n", 16);	
		print_f("%lo\n", 65000000000000000L);
		print_f("%u\n", 12);
		printf("%d%%\n", 15);
*/

int print_f(const char* fmt, ...){
	va_list ap;
	char* p, *sval, cval, ch;
	long long int ival;
	char tmp[100];
	int i, print_num_char = 0;
	int precision = -1; //浮点精度控制
	
	int lflag = 0;
	va_start(ap, fmt);

	for(p = (char*)fmt; *p; p++){
		if(*p != '%'){
			putchar(*p);
			continue;
		}	
		body:
			switch(*(++p)){
				case '%':
					putchar('%');
					print_num_char++;
					break;
				case '.':
					p++;
					for(precision=0; ;){
						precision = precision*10 + (*p) - '0';
						p++;
						if(*p < '0' || *p > '9') break;
					}
					p--;
					goto body;

				case 'l':
					lflag++;
					goto body;

				case 'd':
					if(lflag == 0) itoa(va_arg(ap, int), tmp);
					else if(lflag == 1) itoa(va_arg(ap, long), tmp);
					else itoa(va_arg(ap, long long), tmp);

					for(i=0; tmp[i]; i++){
						putchar(tmp[i]);
						print_num_char++;
					}
					break;

				case 'c':
					//warning: ‘char’ is promoted to ‘int’ when passed through ‘...’ [enabled by default]
					cval = (char)va_arg(ap, int);
					putchar(cval);
					print_num_char++;
					break;
				case 's':
					for(sval = va_arg(ap, char*); *sval; sval++){
						putchar(*sval);
					}
					break;
				case 'u':
					itoa(va_arg(ap, unsigned int), tmp);
					for(i=0; tmp[i]; i++){
						putchar(tmp[i]);
						print_num_char++;
					}
					break;					
				case 'x':
					if(lflag == 0) ival= va_arg(ap, int);
					else if(lflag == 1) ival = va_arg(ap, long);
					else ival = va_arg(ap, long long);
					itob(ival, tmp, 16);
					for(i=0; tmp[i]; i++){
						putchar(tmp[i]);
						
					}
					break;
				case 'o':
					if(lflag == 0) ival= va_arg(ap, int);
					else if(lflag == 1) ival = va_arg(ap, long);
					else ival = va_arg(ap, long long);
					itob(ival, tmp, 8);
					for(i=0; tmp[i]; i++){
						putchar(tmp[i]);
						print_num_char++;
					}	
					break;
				case 'f':
					// warning: ‘float’ is promoted to ‘double’ when passed through ‘...’ [enabled by default]
					if(precision > 0)
						ftoa(va_arg(ap, double), tmp, precision);
					else
						ftoa(va_arg(ap, double), tmp, 6);
					for(i=0; tmp[i]; i++){
						putchar(tmp[i]);
						print_num_char++;
					}
					break;	
				default:
					break;		
		}
	}
	va_end(ap);
	return print_num_char;
}

void* mem_cpy(void* dest, const void* source, unsigned int num){
	assert(dest), assert(source);
	char* cdest = (char*)dest;
	char* csource = (char*)source;
	if(cdest > csource && cdest < csource+num){  //目的地址和源地址有重合
		cdest += num - 1;
		csource += num - 1;
		while(num-- && (*cdest-- = *csource--));

	} else{
		while(num-- && (*cdest++ = *csource++));
	}
	return dest;
}


void* mem_set(void* ptr, int value, unsigned int num){ 
	assert(ptr);
	char* tmp = (char*)ptr;
	char uval = (char)value;
	while(num--){
		*tmp++ = uval;
	}
	return ptr;
}

int str_cmp(char* s, char* t){
	assert(s), assert(t);
	if(s == t) return 0; //自比较	
	for(; *s == *t; s++, t++){
		if(*s == '\0') return 0;
	}
	return *s - *t;
}

int str_cmp_n(char* s, char* t, unsigned int n){
	assert(s), assert(t);
	if(s == t) return 0; //自比较	
	for(; (*s == *t) && n--; s++, t++){
		if(*s == '\0') return 0;
	}
	if(n == 0) return 0;
	return *s - *t;
}


char* str_cpy_n(char* dest, const char* source, unsigned int n){
	assert(dest), assert(source);
	if(dest == source) return dest; //自拷贝
	char* res = dest;
	if(dest > source && dest < source + n){ //地址重合
		dest += n - 1;
		source += n - 1;
		while(n-- && (*dest-- = *source--));
	} else{
		while(n-- && (*dest++ = *source++));
		*dest = '\0';
	}	
	
	return res;
}

char* str_cpy(char* dest, const char* source){
	assert(dest), assert(source);
	if(dest == source) return dest;
	char* res = dest;
	int len = str_len(source);
	if(dest > source && dest < source + len){ //地址重合
		dest += len - 1;
		source += len - 1;
		while(len-- && (*dest-- = *source--));
	} else{
		while(*dest++ = *source++);
	}	

	return res;
}

void itob_core(long long int n, char* str, unsigned int b, int* i){
	if(n >= b) itob_core(n/b, str, b, i);
	str[(*i)++] = "0123456789abcdefghijkopqrstuvwxyz"[n % b];
}

void itob(long long int n, char* str, unsigned int b){
	assert(str), assert(b > 2), assert(b < 36);
	int i = 0;
	itob_core(n, str, b, &i);
	str[i] = '\0';
}


/*
	因为int最大负数为-2147483648,最大正数为2147483647,
	所以不能直接将负数变为正数,再处理
*/
int itoa(long long int n, char* str){
	assert(str);
	int sign = 1;
	char c, i = 0;
	if(n < 0) sign = -1;
	
	do{
		c = (n % 10) * sign + '0';
		str[i++] = c;
	}while((n /= 10) != 0); 

	if(sign == -1) str[i++] = '-';
	str[i] = '\0';
	reverse(str);
	return i;
}


int ftoa(double n, char* str, unsigned int precision){
	assert(str);
	long long int ipart = (long long int)n;
	double fpart = n - ipart;
	char point = '.';
	int ilen = itoa(ipart, str);

	if(precision != 0){
		while(precision--){
			fpart *= 10;
		}

		ipart = (long long int)fpart;
		str_cat(str, &point);
		itoa(ipart, str + ilen + 1);
	}

	return precision+1+ilen;
}

char* reverse(char* str){
	assert(str);
	char* res = str;
	char* str_end = str + str_len(str) - 1;
	char c;
	for(; str < str_end; str++, str_end--){
		c = *str, *str = *str_end, *str_end = c;
	}
	return res;
}

int get_bits(unsigned int x, int pos, unsigned int n){
	return (x >> (pos-n+1) & ~(~0<<n));
}

char* str_cat(char* s, const char* t){
	assert(s), assert(t);
	if(s == t) return s;
 	char* res = s;
	while(*s != '\0') s++;
	while(*s++ = *t++);
	return res;
}

char* str_cat_n(char* s, const char* t, unsigned int n){
	assert(s), assert(t);
	if(s == t) return s;
	char* res = s;
	while(*s != '\0') s++;
	while( n-- && (*s++ = *t++) );
	*s = '\0';
	return res;
}

int atoi_v1(const char* str){
	assert(str);
	int n = 0, i;
	for(i=0; str[i] != '\0'; i++){
		if(str[i] >= '0' && str[i] <= '9'){
			n = n * 10 + str[i] - '0';
		}
	}
	return n;
}

int is_space(char c){
	return (c == ' ') ? 1 : 0;
}

int is_digit(char c){
	return(c >= '0' && c <= '9') ? 1 : 0;
}

int atoi_v2(const char* str){
	assert(str);
	int n=0, i;
	int sign;	
	for(i=0; is_space(str[i]); i++);
	sign = (str[i] == '-') ? -1 : 1;
	if(str[i] == '-' || str[i] == '+') i++;
	for(; is_digit(str[i]); i++){
		n = n * 10 + str[i] - '0';
	}
	return sign*n; 
}

int str_len(const char* str){
	assert(str);
    int i = 0;
    while(str[i]!='\0') {
	i++;
    }
    return i;
}

int get_line(char* line, unsigned int lim){
	assert(line);
    int c, i; 
    for(i = 0; i < lim && (c=getchar()) != EOF && c != '\n'; i++){
        line[i] = c;
    }
    if(c == '\n'){
        line[i] = '\n';
        i++;
    }
    line[i] = '\0';
    return i;
}

void word_count(){
	int num_lines, num_words, num_count;
	int state = OUT;
	int c;
	num_lines = num_words = num_count = 0;
	while((c = getchar()) != EOF){
	    if(c == '\n'){
	        ++num_lines;
	    }
	    ++num_count;
	    
	    if(c == ' ' || c == '\n' || c == '\t'){
	        state = OUT;
	    } else if(state == OUT){
	        state = IN;
	        ++num_words;
	    }
	}
	
	print_f("nl: %d, nw: %d, nc: %d\n", num_lines, num_words, num_count);
}
