#ifndef __UI_MENU_LINK_H
	#define __UI_MENU_LINK_H
	
	/* Admin Link */
	int admin_change_passwd(LinkCarte *link);
	int admin_display_userlist(LinkCarte *link);
	int admin_display_userinfo(LinkCarte *link);
	int admin_add_designer(LinkCarte *link);
	int admin_add_collect(LinkCarte *link);
	
	string admin_add_user();
	
	/* ConcepteurAlbum Link */
	int designer_create_album(LinkCarte *link);	
	int designer_load_album(LinkCarte *link);
	int designer_load_custom(LinkCarte *link);
	int designer_add_card(LinkCarte *link);
	int designer_display_album(LinkCarte *link);

	/* Collectionneur Link */
	int collec_add_collec(LinkCarte *link);
	int collec_add_collec_name(LinkCarte *link, const char *name);
	int collec_load_collect(LinkCarte *link);
	int collec_load_collect_name(LinkCarte *link, const char *name);
	int collec_add_card(LinkCarte *link);
	int collec_check_full(LinkCarte *link);
	int collec_give_card(LinkCarte *link);
	int collec_display_collect(LinkCarte *link);
	int collec_display_bestcard(LinkCarte *link);
	int collec_display_lesscard(LinkCarte *link);
	int collec_display_list(LinkCarte *link);
	int collec_compare(LinkCarte *link);
#endif


