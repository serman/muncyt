/**
 fbo.begin();
 ofRect(0,0,640,480);
 for (int i=0; i< blobTracker.size(); i++){
 cv::Rect blob1Roi = ofxCv::toCv(blobTracker.trackedBlobs[i].angleBoundingRect);  //roi de la zona del blob
 cout << "entro " << i << " --- widt"<< blobTracker.trackedBlobs[i].angleBoundingRect.getWidth() << "\n";
 fullimageMat=ofxCv::toCv(sourceColorImg);
 cv::Mat mat1( fullimageMat,blob1Roi);
 ofxCv::toOf(mat1, blobImg);
 blobImg.draw(blobTracker[i].angleBoundingRect.x,blobTracker[i].angleBoundingRect.y);
 }
 fbo.end();
 //fin intentando enviar solo blobs por syphon
 **/
