﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using CommandLine;


namespace file_searcher
{
	class Application
	{
		public CommandLineCode Exec(string[] args)
		{
			var result=Parser.Default.ParseArguments<Options>(args);
			if (result.Tag == ParserResultType.NotParsed)
			{
				return CommandLineCode.Error;
			}
			_arguments = ((Parsed<Options>)(result)).Value;
			switch (_arguments._path_mode)
			{
				case PathMode.directory:
					return TraverseDirectory();
				case PathMode.file:
					return TraverseFile();
				default:
					//pass
					break;
			}
			return CommandLineCode.Error;
		}

		CommandLineCode TraverseDirectory()
		{
			System.IO.Directory.SetCurrentDirectory(_arguments._working_directory);

			var writer = new System.IO.StreamWriter(_arguments._output_filename, false, Encoding.UTF8);
			var subFolders = Directory.SafeEnumerateFilesInAllDirectories(_arguments._search_path, _arguments._search_pattern);
			var counter = 0;
			//サブフォルダを列挙する
			foreach (string subFolder in subFolders)
			{
				string sub_dir = FormatString(subFolder);
				string abs_dir = System.IO.Path.Combine(_arguments._working_directory,sub_dir);
				writer.WriteLine(string.Format("{0}\t{1}\t{2}", sub_dir ,"@action_directory",abs_dir));
				if (_arguments._delay != 0)
				{
					//(memo) デバッグ用途なのでディスクへ書き込まれるようにする。
					writer.Flush();
					Thread.Sleep(_arguments._delay);
				}

				if (counter == 10)
				{
					//はじめの10行はすぐにファイルへ書き出す
					writer.Flush();
				}
				++counter;
			}

			writer.Close();
			return CommandLineCode.Success;
		}

		string FormatString(string folder)
		{
			if (folder.StartsWith(".\\"))
			{
				//ファイル先頭の.\を取り除く
				folder=folder.Substring(2);
			}
			if (!folder.EndsWith(System.IO.Path.DirectorySeparatorChar.ToString()))
			{
				//\で終端する
				folder = folder+ System.IO.Path.DirectorySeparatorChar;
			}
			return folder;
		}

		CommandLineCode TraverseFile()
		{
			return CommandLineCode.Success;
		}
		Options _arguments;
	}





	class Program
	{
		static int Main(string[] args)
		{
			var app = new Application();
			return (int)app.Exec(args);
		}
	}
}