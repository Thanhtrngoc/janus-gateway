**File Debug.h: 
- cat dat hien thi muc log
- luu cac bien va structure su dung de chia se data giua cac file voi tu khoa extern.
+ bien typeOfMessage de xac dinh xem loai ban tin sdp nao, cua brower 1 hay brower2 can gui dinh

**File janus.c
- Ham janus_process_incoming_request se xu ly tat ca cac ban tin den janus, trong do:
+ Neu message_text la "message" thi se xu ly cac ban tin sdp va request join vao room.
+  Ham "janus_plugin_result *result = plugin_t->handle_message" se gui event sang plugin videoroom xu ly voi ham function callback tuong ung, xu ly o ham janus_videoroom_handler trong janus_videoroomc
. Khi xu ly thanh cong, no se quay lai ham janus_process_incoming_request de tao ban tin phan hoi success cho client.
+ Ham nay se xu ly cac thong tin nhan duoc va luu lai vao structure RoomInfo.
+ Neu message_text la "trickle" thi se xu ly cac ban tin candidate, khi nao gui ban tin candidate complete va ban tin candidate complete do la cua brower nao ? dua vao session_id.

- Ham janus_plugin_push_event la ham xu ly function call back cua push_event khi plugin janus_videoroom.c push_event sang. 
+ Trong ham nay co goi den ham vsm_janus_plugin_handle_sdp (ham goc la janus_plugin_handle_sdp) se thuc hien viec tao ban tin sdp gui den brower. Hien tai, viec tao ban tin sdp la
copy lai ban tin sdp cua brower va add them candidate cua no. Viec sua chinh chi o phan copy bien sdp_merged, cac phan con lai giu nguyen code goc.

Note: - Chua co phan xu ly khi 1 trong 2 brower destroy
		 - Da co ham xu ly viec remove room_id trong list roomInfo va remove toan bo list 
		 
** File ice.c
- Da comment phan xu ly janus_ice_check_failed trong g_source_set_callback(component->icestate_source, janus_ice_check_failed, component, NULL);
- Ham janus_ice_trickle_parse thuc thi parse thong tin trickle candidate nhan duoc. Copy lai candidate va luu vao sdp. Truoc khi tao attribute candidate cho sdp, can phai xoa bo string "candidate:" de tranh gui double "candidate:candidate:"
- Bien CandidateIsCompleted de check khi nao nhan duoc ban tin complete candidate de gui sdp di.
- Do brower 2 co the gui lien tiep 2 ban tiep complete (1 cho complete cua start, 1 cho complete cua configure)  nen phai co co check 

** File janus_videoroom
- Ham janus_videoroom_notify_participants de gui notify den brower ve danh sach cac publisher trong room, cac brower co muon subscriber vao hay khong. Ham  nay se duoc goi o ham janus_videoroom_setup_media
- Ham janus_videoroom_handler thuc hien viec handle cac ban tin sdp, no bao gom 3 phan chinh:
+ janus_videoroom_p_type_none: Khi publiser moi join vao room, chua co bat ki publisher nao trong room, no se nhay vao phan nay. Phan nay se thuc hien viec copy va add thong tin publisher vao list, su dung 
cho viec thong bao den cac client muon join vao room. Khi brower 1 gi ban tin sdp configure se vao phan nay de xu ly, trong nay se co phan gateway->push_event de gui sdp phan hoi lai cho publisher.
+ janus_videoroom_p_type_publisher: 
+ janus_videoroom_p_type_subscriber: 
+ gateway->push_event o gan cuoi ham se xu ly khi nhan duoc ban tin start tu brower 2.
+ Thread vsm_handle_spd_response_for_client se doi cac dieu kien hoan thanh de thuc hien viec gui ban tin sdp den brower. Brower 2 co the gui sdp start va sdp configure den janus theo 2 cach sau:
Cach 1: B2 gui sdp start, gui 1-2 trickle candidate roi gui sdp configure, sau do la 1 loat ban tin trickle candidate va cuoi cung la 2 ban tin trickle candidate complete. Cach 2: B2 gui sdp start, sau do gui
ban tin trickle candidate roi gui trickle completed luon, tiep theo moi gui sdp configure va trickle candidate..., trickle candidate complete. Vi vay phai check (handleStartSdpB2 != NULL) de tranh crash.

Note: Can phai lock thread va unlock thread de tranh crash.
- websocket chay tren port 8188, http tren port 8088
- Khi brower2  gui ban tin trickle candidate chua thong tin port ma no dang open, neu brower 1 gui ban tin sdp va STUN sang cham, no se bi timeout va se khong the stream duoc. Hien tai neu thoi
gian tu luc brower 2 gui candidate den khi no nhan duoc ban tin sdp chua candidate tu brower 1 ma > 10s thi se k stream duoc thanh cong, neu nho hon 10s se stream duoc thanh cong.
- Khi brower 1 vao va send configure sdp, brower 1 phai gui het ban tin candidate completed thi browr 2 moi vao. Chi can check bien isCompletedCandidateOfBrower1 = 1 thi se biet khi nao brower 1
gui candidate complete.
 
