// XML Loading Routine
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
 
ArrayList doLoadXML(String file){ 
  ArrayList returnList = new ArrayList();
  BufferedReader br;
  XML xml;
  xml=null;
  try{
     br = new BufferedReader(new FileReader(dataPath(file)));
     xml = new XML(br);
  } catch (IOException e) {
      e.printStackTrace();
    }   catch( Exception e){
      e.printStackTrace();
    } 

    xml=xml.getChild(1);
    //println(xml.getName());
  //xml=haarcascade_xxx_default
    String sampleSizeStr = (xml.getChildren("size")[0].getContent().trim());//size
    sampleSize = int(split(sampleSizeStr," "));
  //  println(sampleSize);
    XML returnListNode = xml.getChildren("stages")[0];//returnList es stages
    int numReturnList = returnListNode.getChildren("_").length;  
    
  //println("total:" + numReturnList);
  
  for(int i=0;i<numReturnList;i++){//20 returnList in this file //iterate within stages each stage starts with a <_> and a <trees>
    
    XML treesNode = returnListNode.getChildren("_")[i].getChild("trees"); //each tree within treess starts with <_>
    
    int numTrees = treesNode.getChildren("_").length;
    //println("numtrees " + numTrees);
    ArrayList trees = new ArrayList();// holder of nodes

    for(int j=0;j<numTrees;j++){ //iterate over each tree find the nodes inside
      
      XML currentTree=treesNode.getChildren("_")[j];
      //println(currentTree.toString());
      int numNodes = currentTree.getChildren("_").length;    
      //println("nodes inside each tree" + numNodes)  ;
      ArrayList nodes = new ArrayList();
      for(int n=0;n<numNodes;n++){
        XML node = currentTree.getChildren("_")[n];
        XML feature = node.getChild("feature");

        XML rectsNode = feature.getChild("rects");
        int numRects = rectsNode.getChildren("_").length;

        HaarRect[] haarRects = new HaarRect[numRects];
        int[] weights = new int[numRects];

        for(int k=0;k<numRects;k++){
          XML kid = rectsNode.getChildren("_")[k];

          String[] rStr = split(kid.getContent().trim()," ");

          int[] coords = new int[5];

          for(int l = 0;l<coords.length;l++){
            coords[l] = int(rStr[l]); 
          }
      //    println(coords);
          HaarRect hr = new HaarRect(coords);
          haarRects[k] = hr;

        }

        boolean tilted = boolean(feature.getChild("tilted").getContent());
        float leftVal = 0;
        float rightVal = 0;
        int rightNode = 0;
        int leftNode = 0;
        float threshold = float(node.getChild("threshold").getContent().trim());

        for(int l=3;l<node.getChildCount();l++){
          String nName = node.getChild(l).getName();
          XML kid = node.getChild(l);

          if(node.getName().equals("left_val")){
            leftVal = float(kid.getContent().trim());
          } 
          else if (nName.equals("left_node")){
            leftNode = int(kid.getContent().trim());
          } 
          else if(nName.equals("right_val")){
            rightVal = float(kid.getContent().trim());
          } 
          else if(nName.equals("right_node")){
            rightNode = int(kid.getContent().trim());
          }
        }//end for rest of inside the node
        nodes.add(new Node(haarRects,tilted,threshold,leftVal,rightVal,leftNode,rightNode));
       // println("haarRects:  "+haarRects +  " tilted "+ tilted +" threshold: "+threshold+ " leftVal: " +leftVal+ " rightVal: " +rightVal+" leftNode: " +leftNode+" rightNode: "+rightNode);
      }// end nodes loop

      trees.add(new Tree(nodes));

    }//end trees loop
    returnList.add(new Stage(trees));
  }//end returnList loop


  println("");
  println("[cascadeLoader] Total stages: "+returnList.size());
  println("[cascadeLoader] Sample size: "+sampleSize[0] + ", "+sampleSize[1]);
  //println("[cascadeLoader] Rows: "+rows+", Columns: "+cols);
  return returnList;
}



