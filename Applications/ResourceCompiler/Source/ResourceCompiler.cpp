// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "global.h"
#include "Runtime.h"

static UTF8 stringToCStringLiteral(const UTF16& sString)
{
	UTF8 sOut = "L\"";

	const Chr16* p = sString.ptr();

	for (int i = 0; i < sString.length(); i++)
	{
		Chr16 c = p[i];
		Chr16 sC[2];
		sC[0] = c;
		sC[1] = L'\0';

		switch (c)
		{
		case L'\a' : sOut = sOut + "\\a"; break;
		case L'\b' : sOut = sOut + "\\b"; break;
		case L'\f' : sOut = sOut + "\\f"; break;
		case L'\n' : sOut = sOut + "\\n"; break;
		case L'\r' : sOut = sOut + "\\r"; break;
		case L'\t' : sOut = sOut + "\\t"; break;
		case L'\v' : sOut = sOut + "\\v"; break;
		case L'\'' : sOut = sOut + "\\'"; break;
		case L'\"' : sOut = sOut + "\\\""; break;
		case L'\\' : sOut = sOut + "\\\\"; break;
		case L'\?' : sOut = sOut + "\\?"; break;
		default:
			if (0x20 <= c && c <= 0x7E)
				sOut = sOut + UTF8(UTF16(sC));
			else
				sOut = sOut + "\\x" + UTF8::format("%.4X", UInt32(c));
		}
	}

	sOut = sOut + "\"";
	return sOut;
}

class ResourceCompilerWalker : public PathWalker
{
public:
	UTF16List& m_fps;

	ResourceCompilerWalker(UTF16List& fps)
		: m_fps(fps)
	{}

	virtual void startDir(const Path&, bool& bStop)
	{}

	virtual void processFile(const Path& fp, bool& bStop)
	{
		m_fps.add(fp);
	}

	virtual void endDir(const Path&, bool& bStop) {};
};

int wmain(int argc, Chr16** argv)
{
	Lock::setupGlobalLock();

	Error error;
	LogStdout log;
		
	class Fatal {};

	try
	{
		loginfo("Tomazos Resource Compiler 1.0 (c) Andrew Tomazos 2009");

		if (argc != 3)
		{
			error.what(L"Usage: ResourceCompiler <ResourceDir> <Out.cpp>");
			throw Fatal();
		}

		UTF16 sSourceDirPath = argv[1];
		UTF16 sOutPath = argv[2];

		Path sourceDir(error, sSourceDirPath);
		
		if (error)
			throw Fatal();

		UTF16 sFullSourceDirPath = UTF16(sourceDir);
		if (!sFullSourceDirPath.endsWith(L"\\"))
			sFullSourceDirPath = sFullSourceDirPath + L"\\";

		UTF16List fps;
		sourceDir.walk(ResourceCompilerWalker(fps));

		loginfo("Found %d files", fps.size());

		TreeMap<UTF16, Blob> data;

		{
			UTF16List::Iterator it(fps);
			UTF16 sFullFilePath;

			while (it(sFullFilePath))
			{
				if (!sFullFilePath.startsWith(sFullSourceDirPath))
				{
					Check();
					throw Fatal();
				}

				UTF16 sRelFilePath = sFullFilePath.suffix(sFullFilePath.length() - sFullSourceDirPath.length());

				Path file(error, sFullFilePath);
				
				if (error)
					throw Fatal();

				HInputStream fis;
				
				file.readFile(error, fis);

				if (error)
					throw Fatal();

				
				Blob sFileContent = fis->slurp(error);

				if (error)
					throw Fatal();

				loginfo("Found %s (%d bytes)", sRelFilePath.ptr(), sFileContent.length());

				data.add(sRelFilePath, sFileContent);
			}
		}

		Path outPath(error, sOutPath);

		if (error)
			throw Fatal();

		HOutputStream hOut;
		outPath.overwriteFile(error, hOut);

		if (error)
			throw Fatal();

		#define ResLine(x) { hOut->twrite(error, UTF8(x)); if (error) throw Fatal(); hOut->twrite(error, UTF8("\r\n")); if (error) throw Fatal(); }
		#define ResLineF(...) { ResLine(UTF8::format(__VA_ARGS__)); }

		ResLine("#include \"Runtime.h\"");
		ResLine("");
		ResLine("void ResourceManager::setup()");
		ResLine("{");

		{
			TreeMap<UTF16, Blob>::Iterator it(data);
			UTF16 sPath;
			Blob bData;
			int iCount = 0;

			int iNumResources = data.size();

			while (it(sPath, bData))
			{
				iCount++;
				UTF8 sId = UTF8::format("s_res_data_%d", iCount);
				
				ResLineF("    static UInt8 %s[] = { ", sId.ptr());
				for (int i = 0; i < bData.length(); i++)
				{
					if (i % 16 == 0)
					{
						hOut->twrite(error, UTF8("        "));
						if (error)
							throw Fatal();
					}

					hOut->twrite(error, UTF8::format("0x%.2X", UInt32(bData.idx<UInt8>(i))));

					if (error) throw Fatal();

					if (i != bData.length() - 1)
					{
						hOut->twrite(error, UTF8(", "));

						if (error)
							throw Fatal();
					}

					if (i % 16 == 15 || i == bData.length() - 1)
						ResLine("");
				}
				ResLineF("    };");
				ResLine("");
				ResLineF("    ResourceManager::instance()->set(%s, %s, %d);", stringToCStringLiteral(sPath).ptr(), sId.ptr(), bData.length());
				if (iCount != iNumResources)
					ResLine("");
			}
		}

		ResLine("}");

		#undef ResLine

		return 0;
	}
	catch (Fatal&)
	{
		if (error)
			logerr("%s", UTF16(error).ptr());
		
		return 1;
	}
}
