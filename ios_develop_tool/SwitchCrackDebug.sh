#!/bin/bash
if [ $# -ne 1 ] ||  [ $1 != "ON" -a $1 != "OFF" ]; then
echo "useage:./SwitchCrackDebug ON|OFF"
exit 1
fi

export IPHONE_SDK=iPhoneOS7.1.sdk

cp /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/$IPHONE_SDK/SDKSettings.plist SDKSettings.plist
cp /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Info.plist Info.plist
plutil -convert xml1 SDKSettings.plist
plutil -convert xml1 Info.plist
cat SDKSettings.plist | awk -v nextno="0" -v switch=$1 -F \> '{if(nextno=="0"){print $0;if(index($2, "CODE_SIGNING_REQUIRED")!=0 || index($2, "ENTITLEMENTS_REQUIRED")!=0)nextno="1";}else{if(switch=="ON")printf("%s>NO</string>\n",$1);else printf("%s>YES</string>\n", $1);nextno="0";}}' > SDKSettings.plist.mod
mv SDKSettings.plist.mod SDKSettings.plist
cat Info.plist | awk -v switch=$1 -F \> '{if(switch=="ON"){if(index($2, "XCiPhoneOSCodeSignContext")!=0)printf("%s>XCCodeSignContext</string>\n", $1);else print $0;}else{if(index($2, "XCCodeSignContext")!=0)printf("%s>XCiPhoneOSCodeSignContext</string>\n", $1);else print $0;}}' | awk -v switch=$1 -v block=0 -v prefix="" -v found1=0 -v found2=0 -v nextno=no -F \> '{if(index($0, "</dict>")!=0){block=block-1;}if(index($2, "PROVISIONING_PROFILE_ALLOWED")!=0){found1=1;nextno=1;if(switch=="ON")print $0;}else{ if(index($2,"PROVISIONING_PROFILE_REQUIRED")!=0){nextno=1;found2=1;if(switch=="ON")print $0;}else{if(index($0, "/dict")!=0){if(found1==0 && block==0 && switch=="ON")printf("%s<key>PROVISIONING_PROFILE_ALLOWED</key>\n%s<string>NO</string>\n", prefix, prefix);if(found2==0 && block == 0 && switch=="ON")printf("%s<key>PROVISIONING_PROFILE_REQUIRED</key>\n%s<string>NO</string>\n", prefix, prefix);print $0;}else{ if(nextno=="1"){nextno="0";if(switch=="ON")printf("%s>NO</string>\n", $1);}else print $0;}}}prefix = substr($1, 1, index($1, "<")-1);if(index($0,"<dict>")!=0){block=block+1;}}' > Info.plist.mod
mv Info.plist.mod Info.plist
plutil -convert binary1 SDKSettings.plist
plutil -convert binary1 Info.plist
cp SDKSettings.plist /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/$IPHONE_SDK/SDKSettings.plist
cp Info.plist /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Info.plist
if [ "ON" == $1 ]; then
#curl -O http://www.bobzy.cn/xcode/gen_entitlements.txt
#mv gen_entitlements.txt gen_entitlements.py
#sudo chmod 777 gen_entitlements.py
cat $0 | awk -v valid=0 '{if(valid==1)print $0;if(index($0, "exit")==1)valid=1;}' > gen_entitlements.py
export PWD=`pwd`
echo "it's almost done! now do 2 follow things:"
echo "1.now clear your target to Don't Code Sign, clear your target's Provision Profile configure"
echo "2.copy words below and create new script in build phase"
echo '#####################'
echo 'export CODESIGN_ALLOCATE=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate'
echo 'if [ "${PLATFORM_NAME}" == "iphoneos" ] || [ "${PLATFORM_NAME}" == "ipados" ]; then '$PWD'/gen_entitlements.py "my.company.${PROJECT_NAME}" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/${PROJECT_NAME}.xcent";'
echo 'codesign -f -s "iPhone Developer" --entitlements "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/${PROJECT_NAME}.xcent" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/"; '
echo 'fi'
echo '#####################'
fi


exit 0
#!/usr/bin/env python

import sys
import struct

if len(sys.argv) != 3:
	print "Usage: %s appname dest_file.xcent" % sys.argv[0]
	sys.exit(-1)

APPNAME = sys.argv[1]
DEST = sys.argv[2]

if not DEST.endswith('.xml') and not DEST.endswith('.xcent'):
	print "Dest must be .xml (for ldid) or .xcent (for codesign)"
	sys.exit(-1)

entitlements = """
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>application-identifier</key>
    <string>%s</string>
    <key>get-task-allow</key>
    <true/>
</dict>
</plist>
""" % APPNAME

f = open(DEST,'w')
if DEST.endswith('.xcent'):
	f.write("\xfa\xde\x71\x71")
	f.write(struct.pack('>L', len(entitlements) + 8))
f.write(entitlements)
f.close()

