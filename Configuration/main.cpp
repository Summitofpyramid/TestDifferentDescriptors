//
//  main.cpp
//  Configuration
//
//  Created by Dougalas Michael on 16/3/21.
//  Copyright © 2016年 Johnson. All rights reserved.
//
/*
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <string>
#include <vector>
//#include "descriptor.h"
#include "ReidDescriptor.h"
#include "histdescriptor.h"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    
    
    // /Users/dougalasmichael/Documents/ReidDatasets/prid_2011/single_shot
    string inputFolder  = "/Users/dougalasmichael/Documents/ReidDatasets/VIPeR/";
    //  string inputFolder  = "/Users/dougalasmichael/Documents/ReidDatasets/VIPeR/";
    
    
    string defaultProbesFilename  = "probes.txt";
    string defaultGalleryFilename = "gallery.txt";
    string defaultOutputFilename  = "table.csv";
    
    vector<string> List,Singleton; //List is the list of all the images in cam_a
    vector< vector<string> > ClassedList; //
    
    ifstream imgList(inputFolder+defaultGalleryFilename );
    string  line;
    while(getline(imgList,line))
    {
        List.push_back(line);
    
    }
    
    Singleton.push_back(List[0]);
    for(vector< vector<string> >::size_type iter = 1;iter<List.size();++iter)
    {
     //   cout<<List[iter]<<endl;
        
        string Id_previous = List[iter-1].substr(14,4);
        string Id_present = List[iter].substr(14,4);
        if (Id_previous == Id_present )
        
            Singleton.push_back(List[iter]);
  
        else
        {
        
        ClassedList.push_back(Singleton);
        Singleton.clear();
            Singleton.push_back(List[iter]);
        }
        
        if (iter ==(List.size()-1))
        ClassedList.push_back(Singleton);
        
    }
    
    
    cout<<ClassedList.size()<<endl;
    for(vector< vector<string> >::size_type iter1 = 0;iter1<ClassedList.size();++iter1)
    {
        cout<<ClassedList[iter1].size()<<endl;
    }
    
    }
*/

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "descriptor.h"
//#include "ReidDescriptor.h"
#include "histdescriptor.h"
#include "gauss.h"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    // /Users/dougalasmichael/Documents/ReidDatasets/prid_2011/single_shot
    string inputFolder  = "/Users/JohnsonJohnson/Documents/ReidDatasets/VIPeR/";
    //  string inputFolder  = "/Users/dougalasmichael/Documents/ReidDatasets/VIPeR/";
    
    
    string defaultProbesFilename  = "probes.txt";
    string defaultGalleryFilename = "gallery.txt";
    string defaultOutputFilename  = "table.csv";
    
//    //-------------------------------------------
//    string cam_a_mask = "cam_a_mask.txt";
//    string cam_b_mask = "cam_b_mask.txt";
//    //-------------------------------------------
    
    if (argc > 1)
        inputFolder = argv[1];
    
    for (int i = 2; i < argc; i++)
    {
        if (i + 1 != argc)
        {
            if (string(argv[i]) == "-p")
            {
                defaultProbesFilename = argv[i + 1];
            }
            if (string(argv[i]) == "-g")
            {
                defaultGalleryFilename = argv[i + 1];
            }
            if (string(argv[i]) == "-o")
            {
                defaultOutputFilename = argv[i + 1];
            }
            
        }
    }
    
    printf("Dataset: %s\n", inputFolder.c_str());
    printf("probes file: %s\n", defaultProbesFilename.c_str());
    printf("gallery file: %s\n", defaultGalleryFilename.c_str());
    printf("output file: %s\n", defaultOutputFilename.c_str());
    
    
    string output = defaultOutputFilename;
    
    vector<string> probes;
    vector<string> gallery;
    
    TextInput::readLines(inputFolder + defaultProbesFilename, probes);
    TextInput::readLines(inputFolder + defaultGalleryFilename, gallery);
    
    //Implement your own descriptor and change the variable desc
    // Ptr<Descriptor> desc = new ReID();
  //  Ptr<Descriptor> desc = new DescriptorY();
    Ptr<Descriptor> desc = new weightImg();
    
    
    SimilarityTable table(desc, inputFolder, probes, gallery);
    table.createTable();
    table.outputFile(output);
    
    
    return 0;
}