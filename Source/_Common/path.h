#ifndef __PATH_H
#define __PATH_H

//CString MakePath( LPCTSTR lpszFileName );
CString MakePath( LPCTSTR lpszDirName, LPCTSTR lpszFileName );
CString MakePath( LPCTSTR lpDir, DWORD dwLan, LPCTSTR lpFile );
void MakePath( CString& strFullPath, LPCTSTR lpszDirName, LPCTSTR lpszFileName );
void MakePath( TCHAR* lpszFullPath, LPCTSTR lpszDirName, LPCTSTR lpszFileName );
//void SetResourcePath();
//LPCTSTR GetResourcePath();

#define DIR_MUSIC        _T( "Data\\Music\\"   )
#define DIR_SOUND        _T( "Data\\Sound\\"   )
#define DIR_THEME        _T( "Data\\Theme\\Default\\"  )
#define DIR_TEXT         _T( "Data\\Text\\"  )
#define DIR_DIALOG       _T( "Data\\Dialog\\"  )
#define DIR_WORLD        _T( "Data\\World\\"   )
#define DIR_WORLD_GUILDCOMBAT  _T( "Data\\World\\WdGuildWar\\"   )
#define DIR_WORLDPLAY    _T( "Data\\World\\Play\\"  )
#define DIR_WORLDTEX     _T( "Data\\World\\Texture\\"  ) 
#define DIR_WORLDTEXMID  _T( "Data\\World\\TextureMid\\"  ) 
#define DIR_WORLDTEXLOW  _T( "Data\\World\\TextureLow\\"  ) 
#define DIR_WORLDTILE    _T( "Data\\World\\Tile\\"   )
#define DIR_TEXTURE      _T( "Data\\Texture\\" )
#define DIR_FONT         _T( "Data\\Font\\"    )
#define DIR_MODEL        _T( "Data\\"   )
#define DIR_ASE		     _T( "Data\\"   )
#define DIR_MODELTEX     _T( "Data\\"   )
#define DIR_MODELTEXMID  _T( "Data\\"   )
#define DIR_MODELTEXLOW  _T( "Data\\"   )
#define DIR_ICON         _T( "Data\\" )
#define DIR_ITEM         _T( "Data\\" )
#define DIR_EFFECT       _T( "Data\\Effect\\" )
#define DIR_WEATHER      _T( "Data\\"   )
#define DIR_CLIENT       _T( "Data\\"   )
#define DIR_SFX          _T( "Data\\"   )
#define DIR_SFXTEX       _T( "Data\\"   )
#define DIR_LOGO         _T( "Data\\"   )
#ifdef __BS_EFFECT_LUA
#define DIR_SFXLUA		 _T( "Data\\" )
#endif //__BS_EFFECT_LUA
#endif
