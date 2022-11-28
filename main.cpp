#include <iostream>
#include <bits/stdc++.h>

#include "utility.h"

using namespace std;


int workingSet(int frames,vector<int> arr){
    //cout << "Working set: ";
    
    int n= arr.size();
    
    int l=-1,h=-1;
    int wcount=0;
    map<int,int> mp; // map denotes working set size
    int hits=0,miss=0;
    while(wcount<min(frames,n)){
        h++;
        if(mp.find(arr[h])==mp.end()){
            mp[arr[h]]=1;
            miss++;
        }else{
            mp[arr[h]]+=h;
            hits++;
        }
        //cout << "No. of frames : " << mp.size()<<endl;     
        wcount++;
    }
    l++;
    while(h<n-1){
        //cout << "No. of frames : " << mp.size()<<endl;
        h++;
        if(mp.find(arr[h])==mp.end()){
            mp[arr[h]]=1;
            miss++;
        }else{
            hits++;
            mp[arr[h]]+=1;
        }
        if(mp.find(arr[l])!=mp.end()){
            mp[arr[l]]-=1;
            if(mp[arr[l]]==0){
                mp.erase(arr[l]);
            }
            
        }
        l++; 
    }
    // ofstream fout;
	// fout.open("working_set.txt");
    // fout << (double)hits/n;
    // fout.close();
    // cout << "Hits : " << hits << " , Miss : " << miss << endl;
    return hits;
}

int NFU(int nFrame, vector<int> pages)
{
    //cout << "NFU : ";
	
    NFUCache cache(nFrame);
    int i=0;
    for(int p : pages)
    {
        i++;
        if(cache.get(p) == -1)
        {
            cache.miss++;
            cache.put(p, p);
        }
        else
            cache.hits++;
        
    }
    // ofstream fout;
	// fout.open("NFU.txt");
    // fout << (double)cache.hits/nFrame;
    // fout.close();
    // cout << "Hits : " <<  cache.hits << " , Miss : " << cache.miss << endl;
    return cache.hits;
}

int LRU(int nFrame, vector<int> pages)
{
    //cout << "LRU : ";
    LRUCache cache(nFrame);
    for(int p : pages)
    {
        if(cache.get(p) == -1)
        {
            cache.miss++;
            cache.put(p, p);
        }
        else
        {
            cache.hits++;
        }
    }
    // ofstream fout;
	// fout.open("LRU.txt");
    // fout << (double)cache.hits/nFrame;
    // fout.close();
    // cout << "Hits : " <<  cache.hits << ", Miss : " << cache.miss << endl;
    return cache.hits;
}

int aeging(int nFrame, vector<int> pages)
{
    //cout << "Aeging : "; 
    AegingCache cache(nFrame);
    for(int p : pages)
    {
        if(cache.get(p) == -1)
        {
            cache.miss++;
            cache.put(p);
        }
        else
        {
            cache.hits++;
        }
        cache.decrementAgePara();
    }
    // ofstream fout;
	// fout.open("Aging.txt");
    // fout << (double)cache.hits/nFrame;
    // fout.close();
    // cout << "Hits : " <<  cache.hits << ", Miss : " << cache.miss << endl;
    return cache.hits;
}

int FIFO(int frames,vector<int> pageRequests){

    //cout << "FIFO : "; 
    set<int> livePages;
    int maxSize = frames;
    queue<int> holders;
    int pageFaults = 0;
    int pageHits = 0;
    
    for(int i = 0 ; i < pageRequests.size() ; i++){

        if(livePages.find(pageRequests[i]) != livePages.end())
        {
            pageHits++;
            continue;
        }

        if(holders.size() == maxSize){
            int frontElement = holders.front();
            holders.pop();
            livePages.erase(frontElement);
        }

        holders.push(pageRequests[i]);
        livePages.insert(pageRequests[i]);

        pageFaults++;
    }
    // ofstream fout;
	// fout.open("FIFO.txt");
    // fout << (double)pageHits/n;
    // fout.close();
    // cout << "Hits : " << pageHits << " , Miss : " << pageFaults << endl;
    return pageHits;
}

int FIFOWithSecondChance(int frames,vector<int> pageRequests){

    //cout << "FIFO with second chance : "; 
    set<int> livePages;
    int maxSize = frames;
    queue<pair<int,int>> holders; // < Page Number, Reference Bit >
    int pageFaults = 0;
    int pageHits = 0;
    
    for(int i = 0 ; i < pageRequests.size() ; i++){

        if(livePages.find(pageRequests[i]) != livePages.end())
        {
            pageHits++;
            // changeItsReferenceBit to 1
            int n = holders.size();
            for(int j = 0 ; j < n ; j++){

                pair<int,int> node = holders.front();
                
                if(node.first == pageRequests[i])
                {
                    // cout<<"Refrence bit of "<<pageRequests[i]<<" is set to 1"<<endl;
                    node.second = 1;
                }
                
                holders.pop();
                // cout<<node.first<<" is pushed with ref bit "<<node.second<<endl;
                holders.push(node);

            }
            continue;
        }

        else if(holders.size() == maxSize){

            pair<int,int> frontElement = holders.front();
            
            while(frontElement.second != 0){
                holders.pop();
                frontElement.second = 0;
                // cout<<frontElement.first<<" is given second chance and refrence bit is set 0"<<endl;
                holders.push(frontElement);
                frontElement = holders.front();
            }

            holders.pop();
            // cout<<frontElement.first<<" is removed"<<endl;
            livePages.erase(frontElement.first);
        }

        // cout<<pageRequests[i]<<" is Inserted"<<endl;
        holders.push(make_pair(pageRequests[i],0));
        livePages.insert(pageRequests[i]);

        pageFaults++;
    }
    // ofstream fout;
	// fout.open("FIFOSecond.txt");
    // fout << (double)pageHits/n;
    // fout.close();
    // cout << "Hits : " << pageHits << " , Miss : " << pageFaults << endl;
    return pageHits;
}

int CLOCK(int frames,vector<int> pageRequests){

    //cout << "Clock : "; 
    vector<pair<int,int>> holders(frames);
    set<int> livePages;
    int pageHits = 0;
    int pageFaults = 0;
    int maxSize = frames;
    int pages = 0;

    int currIndex = 0;


    for(int i = 0 ; i < pageRequests.size() ; i++){

        if(livePages.find(pageRequests[i]) != livePages.end())
        {
            pageHits++;
            // changeItsReferenceBit to 1
            for(int j = 0 ; j < holders.size() ; j++){

                pair<int,int> node = holders[j];
            
                if(node.first == pageRequests[i])
                {
                    node.second = 1;
                    // cout<<node.first<<" will get a second chance "<<endl;
                    holders[j] = node;
                    break;
                }
            
            }
            continue;
        }

        else if(pages == maxSize) {

            pair<int,int> frontElement = holders[currIndex];
            
            while(frontElement.second != 0){
                frontElement.second = 0;
                // cout<<frontElement.first<<" is made zero"<<endl;
                holders[currIndex] = frontElement;
                currIndex = (currIndex + 1) % maxSize;
                frontElement = holders[currIndex];
            }

            livePages.erase(frontElement.first);
            // cout<<frontElement.first<<" is removed"<<endl;
            
        }

        else{
            pages++;
        }
        
        // cout<<pageRequests[i]<<" is Inserted at "<<currIndex<<endl;
        holders[currIndex] = make_pair(pageRequests[i],0);
        currIndex = (currIndex + 1) % maxSize;
        livePages.insert(pageRequests[i]);

        pageFaults++;



    }
    // ofstream fout;
	// fout.open("Clock.txt");
    // fout << (double)pageHits/n;
    // fout.close();

    // cout << "Hits : " << pageHits << " , Miss : " << pageFaults << endl;
    return pageHits;
}

int WSCLOCK(int frames,vector<int> pageRequests,int threshold){

    //cout << "WSClock : ";
    vector<pair<int,int>> holders(frames);
    set<int> livePages;
    int pageHits = 0;
    int pageFaults = 0;
    int maxSize = frames;
    int pages = 0;
    vector<int> timeStamp(frames);
    int time = 0;

    int currIndex = 0;


    for(int i = 0 ; i < pageRequests.size() ; i++){

        if(livePages.find(pageRequests[i]) != livePages.end())
        {
            pageHits++;
            // changeItsReferenceBit to 1
            for(int j = 0 ; j < holders.size() ; j++){

                pair<int,int> node = holders[j];
            
                if(node.first == pageRequests[i])
                {
                    node.second = 1;
                    // cout<<node.first<<" will get a second chance "<<endl;
                    holders[j] = node;
                    timeStamp[j] = time;
                    time++;
                    break;
                }
            
            }
            continue;
        }

        else if(pages == maxSize) {

            pair<int,int> frontElement = holders[currIndex];
            
            while(frontElement.second != 0 && (time - timeStamp[currIndex]) < threshold){
                time++;
                frontElement.second = 0;
                holders[currIndex] = frontElement;
                currIndex = (currIndex + 1) % maxSize;
                frontElement = holders[currIndex];
            }

            livePages.erase(frontElement.first);
            // cout<<frontElement.first<<" is removed"<<endl;
            
        }

        else{
            pages++;
        }
        
        // cout<<pageRequests[i]<<" is Inserted at "<<currIndex<<endl;
        
        timeStamp[currIndex] = time;
        holders[currIndex] = make_pair(pageRequests[i],0);
        currIndex = (currIndex + 1) % maxSize;
        livePages.insert(pageRequests[i]);
        time++;

        pageFaults++;



    }
    

    //cout << "Hits : " << pageHits << " , Miss : " << pageFaults << endl;

    return pageHits;
}


int random(int min, int max) //range : [min, max]
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

template<typename S>
auto select_random(const S &s, size_t n) {
  auto it = std::begin(s);
  // 'advance' the iterator n times
  std::advance(it,n);
  return it;
}

int randomPRA( vector<int> pages, int frames ){
    int n = pages.size();
    set<int> st;
    int pf = 0, ph = 0; 
    for( int i = 0; i < n; i++ ){
        if( st.size() < frames ){
            if( st.count(pages[i]) == 0 ){
                pf++;
                st.insert(pages[i]);
            }
            else{
                ph++;
            }
        }
        else{
            if( st.count(pages[i]) == 0 ){
                pf++;
                int r_index =  std::rand() % (st.size());
                auto r_num = *select_random(st, r_index);

                //cout << "R: " << r_num << endl;
                st.erase(r_num);
                st.insert(pages[i]);
            }
            else{
                ph++;
            }
        }
    }
    //cout << pf << " " << ph << endl;
    return ph;
}

int optimal( vector<int> pages, int frames ){
    int n = pages.size();
    unordered_map<int,int> mpp;
    int pf = 0, ph = 0; 
    for( int i = 0; i < n; i++ ){
        if( mpp.size() < frames ){
            if( mpp.count(pages[i]) == 0 ){
                pf++;
                mpp[pages[i]] = INT_MAX;
            }
            else{
                ph++;
            }
        }
        else{
            if( mpp.count(pages[i]) == 0 ){
                pf++;
                //auto r_num = select_farthest(mpp);
                for( auto it : mpp ){
                    mpp[it.first] = INT_MAX;
                }
                for( int k = i+1; k < n; k++ ){
                    if( mpp.count(pages[k]) > 0 && mpp[pages[k]] == INT_MAX ){
                        mpp[pages[k]] = k-i;
                        //cout << "...." << endl;
                    }
                }
                //cout << endl;
                int maxi = 0, r_num;
                for( auto it : mpp ){
                    //cout << it.first << " " << mpp[it.first] << endl;
                    if( it.second > maxi ){
                        maxi = it.second;
                        r_num = it.first;
                    }
                }
                //cout << endl;
                //cout << "R: " << r_num << endl;
                mpp.erase(r_num);
                mpp[pages[i]] = INT_MAX;
            }
            else{
                ph++;
            }
        }
        //cout << pages[i] << " " << pf << " " << ph << endl;
    }
    //cout << pf << " " << ph << endl;
    return ph;
}

struct comp {
    template <typename T>
    // Comparator function
    bool operator()(const T& l, const T& r) const
    {
        if (l.second != r.second) {
            return l.second < r.second;
        }
        return l.first < r.first;
    }
};
  

int sort_map(map<int, int>& M)
{
    set<pair<int, int>, comp> S(M.begin(),M.end());
    // for (auto& it : S) {
    //     cout << it.first << ' ' << it.second << endl;
    // }
    int val = -1;
    vector<int> v;
    for (auto& it : S){
        //cout << it.first << " " << it.second << endl;
        if( val == -1 || val == it.second ){
            val = it.second;
            v.push_back(it.first);
        }
    }
    // for( int i = 0; i < v.size(); i++ ){
    //     cout << v[i] << " . ";
    // }
    // cout << endl;
    int r_index =  std::rand() % (v.size());
    int r_num = v[r_index];
    return r_num;
}

int NRU( vector<string> &pages, int frames, int period = 4 ){
    int n = pages.size();
    map<int,int> mpp;
    int pf = 0, ph = 0; 
    int t = period;
    int modify_bit, page;

    for( int i = 0; i < n; i++ ){
        if( pages[i][0] == '$' ){
            modify_bit = 1;
            page = stoi(pages[i].substr(1));
        }
        else{
            modify_bit = 0;
            page = stoi(pages[i]);
        }

        if( t == 0 ){
            for( auto it: mpp ){
                if( it.second >= 2 ){
                    mpp[it.first] = it.second-2;
                }
            }
            t = period;
        }

        if( mpp.size() < frames ){
            if( mpp.count(page) == 0 ){
                pf++;
            }
            else{
                ph++;
            }
            mpp[page] = ((modify_bit == 1)? 3: 2);
        }

        else{
            if( mpp.count(page) == 0 ){
                pf++;
                int r_num = sort_map(mpp);
                // cout << "R: " << r_num << endl;
                mpp.erase(r_num);
                mpp[page] = ((modify_bit == 1)? 3: 2);
            }
            else{
                ph++;
                // if( mpp[pages[i]] == 0 ){
                //     mpp[pages[i]] = ((modify[i] == 1)? 3: 2);
                // }
                if( mpp[page] == 1 || mpp[page] == 3 ){
                    mpp[page] = 3;
                }
                else {//if( mpp[pages[i]] == 2 ){
                    mpp[page] = ((modify_bit == 1)? 3: 2);
                }
                
                
            }
        }
        t--;
        // cout << pages[i] << endl;
        // for (auto& it : mpp) {
        //     cout << it.first << ' ' << it.second << endl;
        // }
        //cout << endl;
    }
    // cout << pf << " " << ph << endl;
    return ph;
}


int main(int argc,char* argv[]){
    // vector<int> arr ={7,0,1,2,0,3,0,4,2,2,2,3,2,1,2,0};

    srand(time(0));
    int type = atoi(argv[1]);
    vector<int> arr;
    vector<string> arr_nru;
    
    int num;
    string str_nru;

    int n;
    int n_nru;
    
    if(type==1){
        //ifstream file;
        int algoNum = atoi(argv[3]);
        if( algoNum != 3 ){
            ifstream file("num.txt");    //Other algo Input
            while (file >> num){
                arr.push_back(num);
            }
            n = arr.size();
        }
        else{
            ifstream file("num_nru.txt");    //NRU Input
            while (file >> str_nru){
                arr_nru.push_back(str_nru);
            }
            n = arr_nru.size();
        }

        int frames=atoi(argv[2]);
        int op;
        switch(algoNum){
            case 1:{
                op=randomPRA(arr,frames);
                ofstream fout;
                fout.open("random.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 2:{
                op=optimal(arr,frames);
                ofstream fout;
                fout.open("optimal.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 3:{
                int period = 4; 
                op=NRU( arr_nru, frames, period);
                ofstream fout;
                fout.open("nru.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 4:{
                op=FIFO(frames,arr);
                ofstream fout;
                fout.open("FIFO.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 5:{
                op=FIFOWithSecondChance(frames,arr);
                ofstream fout;
                fout.open("FIFOSecond.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 6:{
                op=CLOCK(frames,arr);
                ofstream fout;
                fout.open("Clock.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 7:{
                op=LRU(frames,arr);
                ofstream fout;
                fout.open("LRU.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 8:{
                op=NFU(frames,arr);
                ofstream fout;
                fout.open("NFU.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 9:{
                op=workingSet(frames,arr);
                ofstream fout;
                fout.open("working_set.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 10:{
                op=aeging(frames,arr);
                ofstream fout;
                fout.open("Aging.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            case 11:{
                op=WSCLOCK(frames,arr,3);
                ofstream fout;
                fout.open("WSClock.txt");
                fout << (double)op/n;
                cout << "Hit ratio: "<< (double)op/n<<endl;
                fout.close();
                break;
            }
            default:{
                break;
            }

        }
    }else if(type==2){

        //ifstream file, file2;

        ifstream file("num.txt");    //Other algo Input
        while (file >> num){
            arr.push_back(num);
        }
        n = arr.size();
        
        ifstream file2("num_nru.txt");    //NRU Input
        while (file2 >> str_nru){
            arr_nru.push_back(str_nru);
        }
        n_nru = arr_nru.size();


        int l = atoi(argv[2]);
        int h = atoi(argv[3]);
        ofstream outRandom;
        outRandom.open("random.txt");
        ofstream outOptimal;
        outOptimal.open("optimal.txt");
        ofstream outNRU;
        outNRU.open("NRU.txt");
        ofstream outFIFO;
        outFIFO.open("FIFO.txt");
        ofstream outFIFOSecond;
        outFIFOSecond.open("FIFOSecond.txt");
        ofstream outClock;
        outClock.open("Clock.txt");
        ofstream outLRU;
        outLRU.open("LRU.txt");
        ofstream outNFU;
        outNFU.open("NFU.txt");
        ofstream outWorkingSet;
        outWorkingSet.open("working_set.txt");
        ofstream outAging;
        outAging.open("Aging.txt");
        ofstream outWSClock;
        outWSClock.open("WSClock.txt");
        
        int op;
        for(int frame=l;frame<=h;frame++){
            op = randomPRA(arr,frame);
            outRandom << (double)op/n << " ";
            op=optimal(arr,frame);
            outOptimal << (double)op/n << " ";
            
            int period = 4; 
            op=NRU( arr_nru, frame, period);
            //cout << op << endl;
            outNRU << (double)op/n_nru << " ";

            op=FIFO(frame,arr);
            outFIFO << (double)op/n << " ";
            op=FIFOWithSecondChance(frame,arr);
            outFIFOSecond << (double)op/n << " ";
            op=CLOCK(frame,arr);
            outClock << (double)op/n << " ";
            op=LRU(frame,arr);
            outLRU << (double)op/n << " ";
            op=NFU(frame,arr);
            outNFU << (double)op/n << " ";
            op=workingSet(frame,arr);
            outWorkingSet << (double)op/n << " ";
            op=aeging(frame,arr);
            outAging << (double)op/n << " ";
            op=WSCLOCK(frame,arr,3);
            outWSClock << (double)op/n << " ";
        }
    }
    
    return 0;
}