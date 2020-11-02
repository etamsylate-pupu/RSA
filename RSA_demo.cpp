#include <gmpxx.h> //c++ʹ��
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
using namespace std;

#define KEY_LENGTH 2048  //��Կ����
#define BASE 16    //�����������

struct key_pair
{
  char * n;
  char * d;
  char * e;
};
void writefile(char *p,string filename);
mpz_t * gen_primes() //��������������
{
  gmp_randstate_t grt;
  gmp_randinit_default(grt);
  gmp_randseed_ui(grt, time(NULL));
  //High-level signed integer arithmetic functions (mpz)
  mpz_t key_p, key_q; //����һ��gmp�Ĵ�����
  mpz_init(key_p); //��ʼ��
  mpz_init(key_q);

  mpz_urandomb(key_p, grt, KEY_LENGTH / 2); //�����������������
  mpz_urandomb(key_q, grt, KEY_LENGTH / 2);

  mpz_t * result = new mpz_t[2]; //������ɵ�����������
  mpz_init(result[0]);
  mpz_init(result[1]);

  mpz_nextprime(result[0], key_p);  //ʹ��GMP���������ɺ���
  mpz_nextprime(result[1], key_q);
  char * buf_p = new char[KEY_LENGTH + 10];
  char * buf_q = new char[KEY_LENGTH + 10];

  mpz_get_str(buf_p, BASE, key_p); //ת��Ϊ16�����ַ�����ʽ
  writefile(buf_p,"p.txt");
  cout<<"p = "<<buf_p<<endl;
  cout<<"\n"<<endl;
  mpz_get_str(buf_q, BASE, key_q);
  writefile(buf_q,"q.txt");
  cout<<"q = "<<buf_q<<endl;
  cout<<"\n"<<endl;

  mpz_clear(key_p);
  mpz_clear(key_q);

  return result;
}

//Generate key pairs
key_pair * gen_key_pair()
{
  mpz_t * primes = gen_primes();  //��������������p��q

  mpz_t key_n, key_e, key_f,g,s,t,judge;
  mpz_init(key_n);
  mpz_init(key_f);
  mpz_init(g);
  mpz_init(s);
  mpz_init(t);
  mpz_init_set_ui(judge,1);
  mpz_init_set_ui(key_e, 65537);	//ѡ�����(n)������С�ڦ�(n)������e��e����Ϊ65537

  mpz_mul(key_n, primes[0], primes[1]);		//����n=p*q
  mpz_sub_ui(primes[0], primes[0], 1);		//p=p-1
  mpz_sub_ui(primes[1], primes[1], 1);		//q=q-1
  mpz_mul(key_f, primes[0], primes[1]);		//�����(n)=(p-1)*(q-1)
  mpz_gcdext(g,s,t,key_e,key_f);  //�жϦ�(n)��e����������Ƿ�Ϊ1�����жϻ���
  cout<<"g= "<<g<<endl;
    cout<<"\n"<<endl;
  /*cout<<"judge="<<judge<<endl;
  cout<<"\n"<<endl;*/
  if(mpz_cmp(g,judge)!=0){ //��������أ���һֱ������
    mpz_add_ui(key_e,key_e,1);
    mpz_gcdext(g,s,t,key_e,key_f);
  }
  mpz_t key_d;
  mpz_init(key_d);
  mpz_invert(key_d, key_e, key_f); //����d=e-1 (mod ��(n))

  key_pair * result = new key_pair;

  char * buf_n = new char[KEY_LENGTH + 10];
  char * buf_d = new char[KEY_LENGTH + 10];
  char * buf_e = new char[KEY_LENGTH + 10];

  mpz_get_str(buf_n, BASE, key_n ); //ת��Ϊ16�����ַ���
  writefile(buf_n,"n.txt");
  result->n = buf_n;
  mpz_get_str(buf_d, BASE, key_d);
  writefile(buf_d,"d.txt");
  result->d = buf_d;
  mpz_get_str(buf_e, BASE, key_e );
  writefile(buf_e,"e.txt");
  result->e = buf_e;

  mpz_clear(primes[0]);  //�ͷſռ�
  mpz_clear(primes[1]);
  mpz_clear(key_n);
  mpz_clear(key_d);
  mpz_clear(key_e);
  mpz_clear(key_f);
  delete []primes;

  return result;
}

char * encrypt(const char * plain_text, const char * key_n, const char * key_e) //����
{
  mpz_t M, C, n,e;
  mpz_init_set_str(M, plain_text, BASE); //���ַ�����ʼ��Ϊgmp������
  mpz_init_set_str(n, key_n, BASE);
  mpz_init_set_str(e, key_e, BASE);
  mpz_init_set_ui(C, 0);

  mpz_powm(C, M, e, n);    //c=m^e mod n

  char * result = new char[KEY_LENGTH + 10];
  mpz_get_str(result, BASE, C);

  return result;
}


char * decrypt(const char * cipher_text, const char * key_n, const char * key_d) //����
{
  mpz_t M, C, n, d;
  mpz_init_set_str(C, cipher_text, BASE);
  mpz_init_set_str(n, key_n, BASE);
  mpz_init_set_str(d, key_d, BASE);
  mpz_init(M);

  mpz_powm(M, C, d, n);   //M=c^d (mod n)

  char * result = new char[KEY_LENGTH + 10];
  mpz_get_str(result, BASE, M);

  return result;
}


void readfile(char str[], string filename){
  FILE *fp=fopen(filename.c_str(),"r");
  if(fp==NULL){
    printf("file open failed");
    exit(-1);
  }
  while(!feof(fp))
    fscanf(fp,"%s",str);

  fclose(fp);
}

void writefile(char *p,string filename){
  FILE *fp=fopen(filename.c_str(),"w+");
  if(fp==NULL){
    printf("Failed to open the file!");
    exit(-1);
  }
  if(fputs(p,fp)==EOF)printf("Failed to open the file!");
  fclose(fp);
}

string get_value(string key, int argc, char *argv[]){ //��ȡ��Ҫ�������в���ֵ
  for(int i=0; i < argc; i++){
    if(argv[i]==key)return argv[i+1];
  }
  return "";
}

int main(int argc, char *argv[])
{
  string pfile = get_value("-p", argc, argv);
  string nfile = get_value("-n", argc, argv);
  string efile = get_value("-e", argc, argv);
  string dfile = get_value("-d", argc, argv);
  string cfile = get_value("-c", argc, argv);
  if (pfile.empty() && efile.empty() && nfile.empty())printf("cmd line is error!\n");

  key_pair *p=new key_pair;
  char * buf_n = new char[KEY_LENGTH + 10];
  char * buf_d = new char[KEY_LENGTH + 10];
  char * buf_e = new char[KEY_LENGTH + 10];
  readfile(buf_n,nfile);
  p->n=buf_n;
  readfile(buf_d,dfile);
  p->d=buf_d;
  readfile(buf_e,efile);
  p->e=buf_e;

  char buf1[KEY_LENGTH + 10], buf2[KEY_LENGTH + 10];
  if(!(pfile.empty()) && !(cfile.empty())){
     readfile(buf1, pfile); //��ȡ�����ļ�

     cout<<"encryption starts!"<<endl;
     char *cipher_text = encrypt(buf1, p->n, p->e); //����
     cout<<"The ciphertext is:"<<cipher_text<<endl;
     writefile(cipher_text, cfile);
     cout<<"\n"<<endl;

     readfile(buf2, cfile); //��ȡ�����ļ�
     cout<<"decryption starts!"<<endl;
     char* plain_text = decrypt(buf2, p->n, p->d); //����
     cout<<"Plaintext is:"<<plain_text<<endl;
     writefile(plain_text, pfile);

     cout<<"sign starts"<<endl;
     char *sign_text=decrypt(buf1,p->n,p->d);
     cout<<"The signtext is:"<<sign_text<<endl;
     writefile(sign_text, "rsa_sign.txt");
     cout<<"\n"<<endl;
  }
 else
    printf("cmd line is error!\n");
  key_pair * p_test = gen_key_pair(); //����p,q��д��ָ���ļ��ļ�������n,e,d

  cout<<"n = "<<p_test->n<<endl;

  cout<<"\n"<<endl;
  cout<<"d = "<<p_test->d<<endl;
   cout<<"\n"<<endl;
  cout<<"e = "<<p_test->e<<endl;
   cout<<"\n"<<endl;

  return 0;
}
