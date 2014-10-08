//
//  ELP2PChat.h
//  P2PChat
//
//  Created by 刘瀚东 on 14-9-22.
//  Copyright (c) 2014年 eaglelun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "udpclient.h"
#include <vector>
#include <string>
class ChatMsg
{
public:
    NSString* content;
    NSString* sender;
    NSString* reciever;
    NSDate* timestamp;
};
@protocol P2PChatListener <NSObject>

-(void)didRecieveNewChatMsg:(ChatMsg*)msg;

@end

class ChatDispatcher : public P2PClientListener
{
public:
    static const NSString* kOnAddFriend;
    static const NSString* kOnRecvMsg;
	virtual void OnAddPeer(const char* name);
	virtual void OnDelPeer(const char* name);
	virtual void OnRecieveData(const char* name, const char* data, int size);
};

@interface ELP2PChat : NSObject
{
    std::vector<ChatMsg> chatMsgOrderbyTimeStamp;
}

@end
