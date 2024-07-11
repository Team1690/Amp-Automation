#include <iostream>
using namespace std;

enum AmpState {
    NoNotes, 
    OneNotes, 
    TwoNotes, 
    Amplified, 
    ScoreBoard,
    NoChange
};

enum AmpEvent {
    ClockTick,
    NoteIn,
    CoopPressed,
    ScoreBoardPressed,
    AmplifiedPressed
};

class ProcessAmp {
public:
    // defines deafault event functions, to be overriden by state classes
    virtual AmpState NoteIn(){return AmpState::NoChange;};
    virtual AmpState CoopPressed(){return AmpState::NoChange;};
    virtual AmpState ScoreboardPressed(){return AmpState::NoChange;};
    virtual AmpState AmplifiedPressed(){return AmpState::NoChange;};
    virtual AmpState ClockTick(){
        cout << "clock has ticked";
        return AmpState::NoNotes;};
    // handles change from enum to function
    AmpState HandleEvent(AmpEvent e){
        switch (e)
        {
        case AmpEvent::NoteIn:
            return NoteIn();

        case AmpEvent::CoopPressed:
            return CoopPressed();

        case AmpEvent::ScoreBoardPressed:
            return ScoreboardPressed();

        case AmpEvent::AmplifiedPressed:
            return AmplifiedPressed();
        
        default:
            return ClockTick();
        }
    }
};

// state classes override event functions that they use

class NoNotesHandler : public ProcessAmp {
public:
    AmpState NoteIn() override {
        return AmpState::OneNotes;
    }
};

class OneNotesHandler : public ProcessAmp {
public:
    AmpState NoteIn() override {
        return AmpState::TwoNotes;
    }
    AmpState CoopPressed() override {
        return AmpState::NoNotes;
    }
};

class TwoNotesHandler : public ProcessAmp {
public:
    AmpState CoopPressed() override {
        return AmpState::OneNotes;
    }
    AmpState AmplifiedPressed() override {
        return AmpState::Amplified;
    }
};

class AmplifiedHandler : public ProcessAmp {
public:
    AmpState ClockTick() override {
        if (1<3){
            return AmpState::NoNotes;
        }
        return AmpState::Amplified;
    }
};

class ScoreBoardHandler : public ProcessAmp {
public:
    AmpState ScoreboardPressed() override {
        return AmpState::NoNotes;
    }
};


class Context{
public:
    
    NoNotesHandler nnhObj;
    OneNotesHandler onhObj;
    TwoNotesHandler tnhObj;
    AmplifiedHandler ahObj;
    ScoreBoardHandler sbhObj;
    AmpState state = NoNotes;
    int notesIn = 0;

    void HandleEvent(AmpEvent e){
        if (e == NoteIn){notesIn++;}
        switch (state)
        {
            case AmpState::NoNotes:
                state = nnhObj.HandleEvent(e);
                break;

            case AmpState::OneNotes:
                state = onhObj.HandleEvent(e);
                break;
            
            case AmpState::TwoNotes:
                state = tnhObj.HandleEvent(e);
                break;
            
            case AmpState::Amplified:
                state = ahObj.HandleEvent(e);
                break;
            
            case AmpState::ScoreBoard:
                state = sbhObj.HandleEvent(e);
                break;
        }
        cout << state;
    }
};



int main(){
    // starting values for state & event
    AmpEvent event = ClockTick;
    Context c;



    c.HandleEvent(NoteIn);
    c.HandleEvent(NoteIn);
    c.HandleEvent(AmplifiedPressed);


    return 0;
}