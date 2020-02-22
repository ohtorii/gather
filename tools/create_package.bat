@echo off
REM
REM �p�b�P�[�W���쐬����o�b�`�t�@�C��
REM
REM ���g����
REM create_package.bat �o�[�W�����ԍ� �o�͐�f�B���N�g����
REM
REM ���g�p��
REM create_package.bat 1.2.3 c:\project\package
REM

setlocal
set TEMP_TIME=%time: =0%
set NOW=%date:/=%%TEMP_TIME:~0,2%%TEMP_TIME:~3,2%%TEMP_TIME:~6,2%
set ROOT_DIR=%TEMP%\tmp_hm_unity\%NOW%_%random%
set UNITY_ROOT_DIR=%ROOT_DIR%\unity

set RESULT=0
echo ROOT_DIR=%ROOT_DIR%

set ARG_VERSION=%1
set ARG_OUTDIR=%2

call :Main
exit /b %RESULT%



:Main
	REM �����̊m�F
	call :CheckArguments
	if "%errorlevel%" NEQ "0" (
		set RESULT=1
		exit /b 1
	)

	REM �����ɕK�v�ȃR�}���h�����݂��邩���ׂ�
	call :CheckCommand
	if "%errorlevel%" NEQ "0" (
		set RESULT=1
		exit /b 1
	)

	REM ��Ɨp�̈ꎞ�f�B���N�g�������
	md "%ROOT_DIR%"
	if not exist "%ROOT_DIR%" (
		echo �ꎞ�f�B���N�g�����쐬�ł��܂���ł���
		set RESULT=1
		exit /b 1
	)
	REM ��ƃf�B���N�g���ֈړ�����
	pushd "%ROOT_DIR%"
	if "%errorlevel%" NEQ "0" (
		echo pushd�R�}���h�����s���܂���
		set RESULT=1
		exit /b 1
	)

	REM ���C���̏���
	call :Core

	REM ���̃f�B���N�g���ɖ߂�
	popd
	if "%errorlevel%" NEQ "0" (
		echo popd�R�}���h�����s���܂���
		set RESULT=1
		exit /b 1
	)

	REM ��n��
	rmdir /S /Q "%ROOT_DIR%"

	exit /b %errorlevel%


REM �����̊m�F
:CheckArguments
	if "%ARG_VERSION%" == "" (
		echo �o�[�W�����ԍ����w�肵�Ă��������B
		call :Usage
		set RESULT=1
		exit /b 1
	)
	if "%ARG_OUTDIR%" == "" (
		echo �o�̓f�B���N�g�����w�肵�Ă��������B
		call :Usage
		set RESULT=1
		exit /b 1
	)
	exit /b 0


REM �o�b�`�t�@�C�����Ŏg�p����R�}���h�����݂��邩���ׂ�
:CheckCommand
	where git
	if "%errorlevel%" NEQ "0" (
		echo git�R�}���h��������܂���
		set RESULT=1
		exit /b 1
	)

	where 7z
	if "%errorlevel%" NEQ "0" (
		echo 7z�R�}���h��������܂���
		set RESULT=1
		exit /b 1
	)
	exit /b 0



:Core
	REM Unity�}�N����GitHub����擾����
	git clone --recursive --depth 1 "https://github.com/ohtorii/unity.git"
	if not exist "%UNITY_ROOT_DIR%" (
		echo git�R�}���h�����s���܂����B
		set RESULT=1
		exit /b 1
	)

	call :DeleteUnnecessaryFiles
	if "%errorlevel%" NEQ "0" (
		echo aaaaa
		set RESULT=1
		exit /b 1
	)

	REM zip�Ōł߂�
	md "%ARG_OUTDIR%"
	if not exist "%ARG_OUTDIR%" (
		echo �o�̓f�B���N�g�����쐬�ł��܂���ł���
		set RESULT=1
		exit /b 1
	)
	
	7z.exe a -mx9 -mmt%NUMBER_OF_PROCESSORS% "%ARG_OUTDIR%\unity-%ARG_VERSION%.zip" "%UNITY_ROOT_DIR%\"
	if not exist  "%ARG_OUTDIR%\unity-%ARG_VERSION%.zip" (
		echo 7z�R�}���h�����s���܂���
		set RESULT=1
		exit /b 1
	)
	
	exit /b 0



REM �s�v�ȃt�@�C�����폜����
:DeleteUnnecessaryFiles
	rmdir /S /Q  "%UNITY_ROOT_DIR%\.git"
	if exist     "%UNITY_ROOT_DIR%\.git" (
		echo .git �f�B���N�g���̍폜�Ɏ��s���܂����B
		set RESULT=1
		exit /b 1
	)

	rmdir /S /Q  "%UNITY_ROOT_DIR%\tools"
	if exist     "%UNITY_ROOT_DIR%\tools" (
		echo tools �f�B���N�g���̍폜�Ɏ��s���܂����B
		set RESULT=1
		exit /b 1
	)

	rmdir /S /Q  "%UNITY_ROOT_DIR%\help\images\resources"
	if exist     "%UNITY_ROOT_DIR%\help\images\resources" (
		echo help\images\resources �f�B���N�g���̍폜�Ɏ��s���܂����B
		set RESULT=1
		exit /b 1
	)

	rmdir /S /Q  "%UNITY_ROOT_DIR%\internal\dll_project"
	if exist     "%UNITY_ROOT_DIR%\internal\dll_project" (
		echo internal\dll_project �f�B���N�g���̍폜�Ɏ��s���܂����B
		set RESULT=1
		exit /b 1
	)

	exit /b 0


:Usage
	echo;
	echo create_package.bat : Coopyright (c) 2020 ohtorii
	echo;
	echo ���g����
	echo create_package.bat �o�[�W�����ԍ� �o�͐�f�B���N�g����
	echo
	echo ���g�p��
	echo create_package.bat 1.2.3 c:\project\package
	exit /b 0
