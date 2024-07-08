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
    virtual AmpState NoteIn(){return AmpState::NoChange;};
    virtual AmpState CoopPressed(){return AmpState::NoChange;};
    virtual AmpState ScoreboardPressed(){return AmpState::NoChange;};
    virtual AmpState AmplifiedPressed(){return AmpState::NoChange;};
    virtual AmpState ClockTick(){return AmpState::NoChange;};
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


int main(){
    AmpState state = NoNotes;
    AmpEvent event = ClockTick;

    while(true){
        state newState = currentState.HandleEvent(event);
        switch (state)
        {
            case AmpState::NoNotes:
                newState = NoNotesHandler::HandleEvent(event);
                break;

            case AmpState::OneNotes:
                newState = OneNotesHandler::HandleEvent(event);
                break;
            
            case AmpState::TwoNotes:
                newState = TwoNotesHandler::HandleEvent(event);
                break;
            
            case AmpState::Amplified:
                newState = AmplifiedHandler::HandleEvent(event);
                break;
            
            case AmpState::ScoreBoard:
                newState = ScoreBoardHandler::HandleEvent(event);
                break;


        }
    }
    return 0;
}