#include <iostream>
#include <string>
#include <vector>
#include <iomanip> 
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
int block_size, cash_size,number_of_blocks=0;; //블록크기 등 선언
int compulsry_misses=0, capcity_misses=0, conflict_misses=0;

#define		DRAM_SIZE		(64*1024*1024)

unsigned int m_w = 0xABABAB55;    /* 0이면 안됨 */
unsigned int m_z = 0x05080902;    /* 0이면 안됨 */

unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  //32비트 결과

unsigned int memGen1(){    static unsigned int addr=0;    return (addr++)%(DRAM_SIZE);}

unsigned int memGen2(){    static unsigned int addr=0;    return  rand_()%(128*1024);}

unsigned int memGen3(){    return rand_()%(DRAM_SIZE);}

unsigned int memGen4(){    static unsigned int addr=0;    return (addr++)%(1024);}

unsigned int memGen5(){    static unsigned int addr=0;    return (addr++)%(1024*64);}

unsigned int memGen6(){    static unsigned int addr=0;    return (addr+=256)%(DRAM_SIZE);}

bool cachesimulator(unsigned int addr, int cash[3][100000], int type, int &block_counter, int index_addr, int tag_addr) // 캐시 시뮬레이터 함수
{
    int shift_offset=log2(block_size);
    bool detected=false;
    bool misses_flag=true;    
      
    if (cash[0][index_addr]==tag_addr)
    {
        return true;
    }
    else
    {
        cash[0][index_addr]= tag_addr;
        for (int i=0; i < number_of_blocks; i++)
        {
            if (cash[1][i]!=1){misses_flag=false; i=number_of_blocks;}                 
        }
        //계산 미스 발생시
        if (misses_flag)
            capcity_misses++;   // 캐퍼시티 미스가 발생, 캐시가 꽉차서
        else
        {
            if(cash[1][index_addr]==1)
                conflict_misses++;
            else
            {
                compulsry_misses++;
            }
        }
        cash[1][index_addr]= 1;
        return 0;
    }
    return true;
}

char *msg[2] = {"Miss","Hit"};

int main(int argc, const char * argv[]) {
    
    int looper=1000000, addr, flag, shift;

    cout << "블록 사이즈 입력  :" << endl;
    cin >> block_size;
    cout << "캐시 사이즈 입력: 1KB – 64KB사이; " << endl;
    cin >> cash_size;   

    int cash[3][100000];
    int block_counter=0;
    int hit_counter=0;
    int index_addr=0, tag_addr=0;
    
    //캐시 계산
    
    number_of_blocks= (cash_size*1024)/block_size;
    
    
    for (int i=0; i < 2; i++)   // 모든 캐시를 -1로 설정
        for (int j=0; j < number_of_blocks; j++)
            cash[i][j]=-1;     
        
    for(int i=0; i <looper ;i++)
    {
        addr = memGen1();
        shift= log2(block_size);
        index_addr= (addr >> shift)% number_of_blocks;
        shift= log2(number_of_blocks+block_size);
        tag_addr= addr >>shift;    // 오프셋 및 인덱스 크기를 이동
        flag = cacheSim(addr, cash, 0,block_counter, index_addr, tag_addr);
        index_addr=0;
        tag_addr=0;
        cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[flag] <<")\n";
        if (msg[flag]=="Hit")
        {
            hit_counter++;
        }
    }
    cout << "Hits  " << hit_counter<<endl << "Compulsry:  " << compulsry_misses <<endl<< "Capcity:  " << capcity_misses <<endl<< "Conflict:  " << conflict_misses <<  endl; 
}


