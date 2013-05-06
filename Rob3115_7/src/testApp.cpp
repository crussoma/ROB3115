#include "testApp.h"

/* -- ROB3115 - A Neuro Immersive Narrative ------------------------------------------/
 
 System Modes:
 0 = Intro Screen (sequence)
 1 = Connecting to Device
 2 = Mini-game Splash Screen
 3 = Story (Series of Chapters and Pages)
 4 = Close Sequence
 
 */

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetFullscreen(TRUE);
    ofSetCircleResolution(200);
    
    cout << "hello world." << endl;
    
    mode = -1;
    font1.loadFont("font/venus_rising.ttf", 12);
    connectText = false;
    
    number = 1;
//    intro.loadImage("Intro_slide"+ofToString(number)+".png");
    intro.loadImage("MAIN_NEW.jpg");
    
//    minigame.setup();
    
    storySetUp = false;
    
    chapterAttValue = 0.0;
    currentAttValue = 0.0;
    blink = 0;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
//    cout << "MODE: " << mode << endl;
    
    //TEST MODE---------------------
    if(mode==100){
        story.update(0,100);
        
    }
    //END TEST MODE-----------------
    
    
    if(mode==3){ //Story
        
        blink = eeg.updateNew(); // 1 = single 2 = double  ... each time EEG updates determines if there is a single or double blink to be sent to story.turnPage
        cout << "BLINK: " << blink << endl;
        chapterAttValue = eeg.getChapterAttention();
        if(eeg.attValues.size()>0){
            currentAttValue = eeg.attValues[eeg.attValues.size()];
        }
        currentSignalQuality = eeg.signalQuality;
        
        cout << "Signal Quality TestApp: " << currentSignalQuality << endl;
        
        story.update(currentAttValue, currentSignalQuality);
//        story.turnPage(blink, chapterAttValue); // turn on to activate BLINK TURN PAGE
        story.turnPage(0, chapterAttValue);
//        blink = 0;
    
    }
    
    if(mode==2){ //Minigame
        
//        minigame.update();
        
        minigame.update();
        if(minigame.textDone){
            blink = eeg.updateNew(); // 1 = single 2 = double
            cout << "BLINK: " << blink << endl;
            if(blink==2){
                mode = 3;
//                blink = 0;
            }
        }
    }
    
    
    if(mode==1 && connectText==true){ //Connecting to EEG
        bool connection;
        
        try {
            connection = eeg.setup(deviceNumber);
//            throw false;
        } catch (bool _notSureHowToUseThis) {
            connection = false;
            cout << "Failed Attempt... Trying Choosing Another Device or Restarting Your Current Device." << endl;
        }
//
//        connection = eeg.setup(deviceNumber);
        
        if (connection==true) {
            minigame.setup();
            mode=2;
            
            
        }
        else{
            mode = 0;
            
            cout << "Try Again" << endl;
            font1.drawString("Try Again", 100, 100);
            connectText = false;
        }
        
//        loadScreen.stopThread();
        
    }
    
    //give slight delay before story setup to make application open more quickly... 
    if(mode==0 && storySetUp==false && ofGetFrameNum() > 1){
        
//    if(mode==0 && storySetUp==false){
        cout << "FRAME: " << ofGetFrameNum() << endl;
        cout << "ofGetWidth: " << ofGetWidth() << endl;
        cout << "ofGetWindowWidth: " << ofGetWindowWidth() << endl;
        cout << "screenWidth: " << ofGetScreenWidth() << endl;
        
        story.setup();
        storySetUp = true;
    }
    
    if(mode==-1){
        if(ofGetElapsedTimeMillis() > 4000){
            mode = 0;
        }
        
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //TEST MODE----------------------
    if(mode == 100){
        story.draw(blink);
        blink = 0;
        
    }
    //END TEST MODE------------------
    
    if(mode==3){
        //        story.draw(blink); // turn on to activate blink visual
        story.draw(0);
        eeg.draw();
        blink = 0; //reset for new reading in next frame (to be read in update as well)
    }
    
    if(mode==2){
        minigame.draw();
        eeg.draw();
        blink = 0;
    }
    
    if(mode==1){
        ofPushStyle();
        ofSetColor(0);
//        ofRect(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
        ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        ofSetColor(255);
        font1.drawString("Connecting to Device", 100, 100);
        ofPopStyle();
        connectText = true;
    }
    
    if(mode==0){
//        intro.draw(0,0, ofGetScreenWidth(), ofGetScreenHeight());
        intro.draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    }
    
    ofSetWindowTitle("fps = " + ofToString(ofGetFrameRate()));
    
    if(mode==-1){
        intro.draw(0,0, ofGetWidth(), ofGetHeight());
        ofPushStyle();
        ofSetColor(0, 0, 0, ((4000.0-ofGetElapsedTimeMillis())/4000.0)*255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    if(mode == 100){
        if(key==' '){
            chapterAttValue = eeg.getChapterAttention();
            story.turnPage(blink, chapterAttValue);
        }
        
    }
    
    if(mode==3){
        if(key==' '){
            
            blink = 1;
            chapterAttValue = eeg.getChapterAttention();
            story.turnPage(blink, chapterAttValue);
            blink = 0;
        }
        
    }
    
    if(mode>1){
        if(key=='g'){
            eeg.graphOn = !eeg.graphOn;
        }
    }
    
    if(mode == 0){  //connect without EEG
        minigame.setup();
        mode = 100;
        
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
    if(mode==100){
        
        minigame.randomizeQuote();
    }
    
    if(mode==3){
        story.turnPage(2, chapterAttValue);
    }
    
    if(mode==2){
        mode=3;
        
    }
    
    if(mode==0){
        if(mouseX>=(897.0/1280.0)*ofGetWidth() && mouseX <= (999.0/1280.0)*ofGetWidth() && mouseY >= (653.0/768.0)*ofGetHeight() && mouseY <= (754.0/768.0)*ofGetHeight()){ //select Mindwave Mobile on intro screen
            mode=1;
            deviceNumber = 1;
//            loadScreen.startThread(false,true);
            
        }
        if(mouseX>=(1028.0/1280.0)*ofGetWidth() && mouseX <= (1129.0/1280.0)*ofGetWidth() && mouseY >= (653.0/768.0)*ofGetHeight() && mouseY <= (754.0/768.0)*ofGetHeight()){ //select Mindset on intro screen
            mode=1;
            deviceNumber = 2;
        }
        if(mouseX>=(1157.0/1280.0)*ofGetWidth() && mouseX <= (1259.0/1280.0)*ofGetWidth() && mouseY >= (653.0/768.0)*ofGetHeight() && mouseY <= (754.0/768.0)*ofGetHeight()){ //select Mindband
            mode=1;
            deviceNumber = 3;
        }
    }
    cout << "XXXXXXXXXXXXXXXXX mouseX: " << mouseX << " || mouseY: " << mouseY << endl;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}